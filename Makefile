CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -D_POSIX_SOURCE

all:	proxy

proxy:	proxy.c wProxy.h
	$(CC) $(CFLAGS) proxy.c -o proxy

clean:
	rm -f proxy
