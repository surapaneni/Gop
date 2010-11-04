DIRS=	src

all:
	for A in $(DIRS); do $(MAKE) -C $$A; done

clean:	
	rm -f src/Gop
	rm -f src/*.o

.PHONY: all clean
