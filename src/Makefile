SRC= main.c server.c util.c event.c

CC=	gcc
CFLAGS+=	-g -std=c99 -Wall -pedantic -D_POSIX_C_SOURCE=1 -DDEBUG

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
