SRC= main.c server.c util.c event.c http_parser.c http_action.c

CC=	gcc
CFLAGS+= -O0 -g -std=gnu99 -Wall -Werror -D_POSIX_C_SOURCE=1 -I.

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
