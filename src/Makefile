SRC= main.c server.c util.c

CC=	gcc
CFLAGS+=	-g -std=c99 -Wall -pedantic -D_POSIX_C_SOURCE=1 -DDEBUG

Gop:	clean $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) 

clean:
	rm -f Gop
	rm -f *.o

.PHONY: clean
