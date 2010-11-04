DIRS=	src

all:
	for A in $(DIRS); do $(MAKE) -C $$A; done

clean:	
	for A in $(DIRS); do $(MAKE) -C $$A clean; done

.PHONY: all clean
