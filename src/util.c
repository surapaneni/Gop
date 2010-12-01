#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "util.h"

void get_max_fds(unsigned int * _max_fds) {
	struct rlimit rlp;
	
	rlp.rlim_cur = rlp.rlim_max = RLIM_INFINITY;
	getrlimit(RLIMIT_NOFILE, &rlp);

	*_max_fds = rlp.rlim_cur;
}

int set_nonblock(int fd) {
	return fcntl(fd, F_SETFD, O_NONBLOCK);
}
