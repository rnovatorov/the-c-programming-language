SHELL=/bin/sh
CC = gcc
LIB = lib
DST = bin

$(patsubst %.c,%,$(wildcard *.c)):
	$(CC) $@.c $(LIB)/* -o $(DST)/$@

clean:
	rm -v $(DST)/*
