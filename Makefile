# Set compiler and its options
CC=gcc
CFLAGS=-g -Wall -Werror --std=c99 -O3 -D_POSIX_C_SOURCE=200112L
DEBUG=-DDEBUG
LDFLAGS=-I lib/ 
VERSION=0.0.1

all: test

test: test-yacsvl.c 
	$(CC) $(CFLAGS) $(LDFLAGS) test-yacsvl.c -o test-yacsvl -lyacsvl

install: shared
	cp libyacsvl.so.$(VERSION) /usr/local/lib/
	chmod 0755 /usr/local/lib/libyacsvl.so.$(VERSION)
	ln -s /usr/local/lib/libyacsvl.so.$(VERSION) /usr/local/lib/libyacsvl.so
	ldconfig

shared: yacsvl.o
	$(CC) -shared -Wl,-soname,libyacsvl.so.$(VERSION) -o libyacsvl.so.$(VERSION) yacsvl.o -lc

yacsvl.o: src/yacsvl.c lib/yacsvl.h
	$(CC) -fPIC $(CFLAGS) $(LDFLAGS) -c src/yacsvl.c

clean:
	rm -rf test-yacsvl *.o *.so *.so.*
