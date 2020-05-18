SHELL=/bin/sh
CC = gcc
LIB = lib
TESTS = tests
DST = bin

all:
	$(MAKE) $(patsubst %.c,%,$(wildcard *.c))

$(patsubst %.c,%,$(wildcard *.c)):
	$(CC) -g $(LIB)/*.c $@.c -o $(DST)/$@

$(patsubst $(TESTS)/%.c,%,$(wildcard $(TESTS)/*.c)):
	$(CC) $(LIB)/*c $(TESTS)/$@.c -o $(DST)/$@ && $(DST)/$@

clean:
	rm -v $(DST)/*
