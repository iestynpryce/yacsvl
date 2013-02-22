# Set compiler and its options
CC=gcc
CFLAGS=-g -Wall -Werror --std=c99 -O3 -D_POSIX_C_SOURCE=200112L
DEBUG=-DDEBUG
LDFLAGS=-I lib/

all: test

test: test-yacsvl.c yacsvl.o
	$(CC) $(CFLAGS) $(LDFLAGS) yacsvl.o test-yacsvl.c -o test-yacsvl

yacsvl.o: src/yacsvl.c lib/yacsvl.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c src/yacsvl.c

clean:
	rm -rf test-yacsvl *.o
