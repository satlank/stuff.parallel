include Makefile.config

.PHONY: all clean

all:
	make matmul_basic

clean:
	rm -f matmul_basic matmul_basic.o matrix.o

matmul_basic:	matmul_basic.o matrix.o
	$(CC) $(CFLAGS) -o matmul_basic matmul_basic.o matrix.o

matmul_basic.o:	matmul_basic.c matrix.h config.h
	$(CC) $(CFLAGS) -c -o matmul_basic.o matmul_basic.c

matrix.o:	matrix.c matrix.h config.h
	$(CC) $(CFLAGS) -c -o matrix.o matrix.c

