CC = gcc
DST = bin

$(patsubst %.c,%,$(wildcard *.c)):
	$(CC) $@.c -o $(DST)/$@
