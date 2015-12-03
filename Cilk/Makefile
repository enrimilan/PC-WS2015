# Makefile for Cilk Merge
# Author: Stefan Karner (1027209)
# Date: 03.12.2015


PROG = cilk_merge

CCLD = gcc
CFLAGS = -Wall -fcilkplus -O3 -g -std=c99 -pedantic


all: $(PROG)

%.o: %.c
	$(CCLD) $(CFLAGS) -c $^

$(PROG): $(PROG).o
	$(CCLD) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(PROG).o $(PROG)

.PHONY: all clean