#ifndef __GOP_EVENT_H__
#define __GOP_EVENT_H__

#include <sys/epoll.h>
int event_register_new(int,struct epoll_event *,int);
int event_deregister(int,int);
#endif
