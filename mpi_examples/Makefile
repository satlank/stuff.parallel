include Makefile.config

.PHONY: all clean

all:
	make hello vector mpitests

clean:
	rm -f hello hello.o mpitests mpitests.o vector vector.o

hello: hello.o
	$(CC) $(CFLAGS) -o hello hello.o

hello.o:	hello.c  config.h
	$(CC) $(CFLAGS) -c -o hello.o hello.c

mpitests: mpitests.o
	$(CC) $(CFLAGS) -o mpitests mpitests.o

mpitests.o:	mpitests.c  config.h
	$(CC) $(CFLAGS) -c -o mpitests.o mpitests.c

vector: vector.o
	$(CC) $(CFLAGS) -o vector vector.o -lm

vector.o:	vector.c  config.h
	$(CC) $(CFLAGS) -c -o vector.o vector.c
