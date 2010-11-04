#include <stdlib.h>

#include "server.h"

int main(int argc,char * argv[]) {
	server_t * server;

	server = gop_init(argc,argv);

	if(NULL != server) {
		gop_serve_forever(server);
		gop_cleanup(server);
		return 0;
	}
	return 1;
}
