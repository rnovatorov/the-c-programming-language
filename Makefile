CC = gcc
DST = bin

$(TARGET):
	$(CC) $@.c -o $(DST)/$@
