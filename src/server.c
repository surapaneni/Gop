#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

#include "event.h"
#include "server.h"
#include "util.h"
#include "http_parser.h"
#include "http_action.h"

server_t * gop_init(int argc,char ** argv) {
	int g,reuse_addr = 1;
	struct addrinfo hints,*res0,*res;
	struct epoll_event server_event;
	server_t * s;
	char * port;

	s = (server_t *)malloc(sizeof(server_t));
	if(!s)
		return NULL;
	
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = 0;
	if(!argv[1])
		return NULL;
	if(argv[2])
		port = argv[2];
	else
		port = "8080";
	g = getaddrinfo(argv[1],port,&hints,&res0);
	if(g) {
		fprintf(stderr,"%s\n",gai_strerror(g));
		return NULL;
	}

	for(res = res0; res != NULL; res = res->ai_next) {
		int sfd;

		sfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(sfd == -1) {
			perror("gop_init -> socket()");
			continue;
		}
		setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
		if(bind(sfd,res->ai_addr,res->ai_addrlen) == 0) {
			s->addr = res;
			s->sockfd = sfd;
			break;
		}

		close(sfd);
	}

	if(!res) {
		fprintf(stderr,"cannot bind()\n");
		return NULL;
	}

	s->efd = epoll_create(1);
	if(s->efd < 0) {
		perror("gop_init -> epoll_create()");
		return NULL;
	}

	set_nonblock(s->sockfd);
	server_event.data.fd = s->sockfd;
	server_event.events = EPOLLET | EPOLLIN;
	if( -1 == epoll_ctl(s->efd,EPOLL_CTL_ADD,s->sockfd,&server_event))
		return NULL;
	return s;
}

void gop_cleanup(server_t * s) {
#ifdef DEBUG
	fprintf(stderr,"gop_cleanup()\n");
#endif
	freeaddrinfo(s->addr);	
	close(s->sockfd);
	close(s->efd);
	free(s);
}

void gop_serve_forever(server_t * s) {
	int nfds;
	int rv;
	unsigned int max_fds;
	struct epoll_event ev, *events;

	rv = listen(s->sockfd,GOP_LISTEN_BACKLOG);
	if(rv) {
		perror("gop_serve_forever -> listen()");
		return;
	}

	get_max_fds(&max_fds);

#ifdef DEBUG
	fprintf(stderr,"Max allowed FD's: %u\n",max_fds);
#endif

	events = (struct epoll_event *)calloc(max_fds,sizeof(struct epoll_event));

	if(!events) 
		return;
	
	for(;;) {
		nfds = epoll_wait(s->efd,events,max_fds,-1);
		if(-1 == nfds) {
			perror("gop_serve_forever -> epoll_wait()");
			return;
		}

		for(int i=0; i< nfds; i++) {
			if(events[i].data.fd == s->sockfd) {
				//Server Sock available for read
				//Accept a new client and add it to epoll
				event_register_new(s->efd, &ev, s->sockfd);
			} else {
				if(events[i].events & ( EPOLLIN | EPOLLOUT)) {
					http_parser_settings settings;
					memset(&settings,0,sizeof(http_parser_settings));
					//settings.on_header_field = on_header_field;
					settings.on_body = on_body;
					http_parser *parser = (http_parser *)malloc(sizeof(http_parser));
					http_parser_init(parser, HTTP_REQUEST);
					parser->data = (int*)&(events[i].data.fd);
					char request[HTTP_MAX_HEADER_SIZE];
					int rv = read(events[i].data.fd,request,HTTP_MAX_HEADER_SIZE);
					if(rv < 0)
						perror("gop_serve_forever -> read");
#ifdef VERBOSE_DEBUG
					else
						fprintf(stdout,"Read: %s",request);
#endif
					http_parser_execute(parser,&settings,request,rv);
					// The following can be used as an example as to where we are going with.
					char reply[] = "HTTP/1.1 204 OK\r\nConnection: close\r\nContent-Length: 0\r\n\r\n";
					rv = write(events[i].data.fd,reply,strlen(reply));
					if(rv < 0)
						perror("gop_serve_forever -> write");
#ifdef VERBOSE_DEBUG
					else
						fprintf(stdout,"Wrote: %s",reply);
#endif
					free(parser);
				}
				if(events[i].events & ( EPOLLHUP | EPOLLERR )) {
#ifdef DEBUG
					fprintf(stderr,"Will close(2) client\n");
#endif
					event_deregister(s->efd,events[i].data.fd);
					close(events[i].data.fd);
				}
			}
		}
	}

	free(events);
	return ;
}
