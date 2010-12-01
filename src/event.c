#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "event.h"
#include "util.h"

int event_register_new(int efd, struct epoll_event * ev, int sockfd) {
	struct sockaddr client;
	int cli_sock;
	socklen_t sock_len = sizeof(struct sockaddr);

	cli_sock = accept(sockfd, &client, &sock_len);

	if(cli_sock < 0) {
		return -1;
	}

	set_nonblock(cli_sock);
	ev->events = EPOLLIN | EPOLLOUT | EPOLLET;
	ev->data.fd = cli_sock;

	if( -1 == epoll_ctl(efd, EPOLL_CTL_ADD, cli_sock, ev)) {
		return -1;
	}

	return 0;
}

int event_deregister(int efd, int fd) {
	return epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);
}

