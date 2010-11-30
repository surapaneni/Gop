#include <signal.h>
#include <stdlib.h>

#include "server.h"

int main(int argc,char * argv[]) {
	server_t * gop_handle;
	struct sigaction act;

	gop_handle = gop_init(argc,argv);
	
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGPIPE,&act,NULL);

	if(gop_handle) {
		gop_serve_forever(gop_handle);
		gop_cleanup(gop_handle);
		return 0;
	}
	return 1;
}
