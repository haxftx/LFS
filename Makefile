#!/bin/bash
CC=gcc
CFLAGS= -Wall -lm -O3
INFILE = input.in
OUTFILE = output.out
EXEC = lfs
FILE = lfs.c

build: $(EXEC)

lfs.o:lfs.c
	$(CC) $(CFLAGS) -o lfs.o -c $(FILE)
lfs:lfs.o
		$(CC) $(CFLAGS) -o lfs lfs.o
run:$(EXEC)
		./$(EXEC) < $(INFILE) > $(OUTFILE) 2> error.out

.PHONY: clean checkMem

clean:
	rm -f *.o $(EXEC)

