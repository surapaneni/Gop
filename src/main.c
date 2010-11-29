#include <stdlib.h>

#include "server.h"

int main(int argc,char * argv[]) {
	server_t * gop_handle;

	gop_handle = gop_init(argc,argv);

	if(gop_handle) {
		gop_serve_forever(gop_handle);
		gop_cleanup(gop_handle);
		return 0;
	}
	return 1;
}
