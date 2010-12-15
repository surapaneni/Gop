SRC= main.c server.c util.c event.c http_parser.c http_action.c http_handle.c \
	http_request.c

CC=	gcc
CFLAGS+= -O0 -g -std=gnu99 -Wall -Werror -DPOSIX_C_SOURCE=200809L  -D_POSIX_C_SOURCE=200809L -I. 

all:	clean Gop tags

Gop:	$(patsubst %.c,%.o,$(SRC))	
	$(CC) $(CFLAGS) $^ -o $@
	
%.o:	%.c	
	$(CC) -c $(CFLAGS) $< 

tags:	$(wildcard *.c) $(wildcard *.h)	
	ctags $^
clean:
	rm -f Gop
	rm -f *.o
	rm -f tags

.PHONY: all clean
