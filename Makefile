SHELL=/bin/sh
CC = gcc
LIB = lib
TESTS = tests
DST = bin

$(patsubst %.c,%,$(wildcard *.c)):
	$(CC) $(LIB)/*.c $@.c -o $(DST)/$@

$(patsubst $(TESTS)/%.c,%,$(wildcard $(TESTS)/*.c)):
	$(CC) $(LIB)/*c $(TESTS)/$@.c -o $(DST)/$@ && $(DST)/$@

clean:
	rm -v $(DST)/*
