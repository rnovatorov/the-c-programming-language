SHELL=/bin/sh
CC = gcc
DST = bin

$(patsubst %.c,%,$(wildcard *.c)):
	$(CC) $@.c -o $(DST)/$@

clean:
	rm -v $(DST)/*
