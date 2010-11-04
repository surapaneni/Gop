#ifndef __GOP_SERVER_H__
#define __GOP_SERVER_H__

#define GOP_LISTEN_BACKLOG 64

typedef struct server_t server_t;

struct server_t {
	int sockfd; /* server socket fd */
	int efd; /* epoll fd */
	struct addrinfo * addr; /* server addrinfo */
};

server_t * gop_init(int,char **);
void gop_serve_forever(server_t *);
void gop_cleanup(server_t *);
int set_nonblock(int);
#endif
