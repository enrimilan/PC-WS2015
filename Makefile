# Makefile for the testsuite
# Date: 06.12.2015


PROG = main
IMPL_PATH = merge
IMPLS = sequential_merge cilk_merge omp_merge mpi_merge

CCLD = gcc
CFLAGS = -Wall -fcilkplus -fopenmp -O3 -g -std=c99 -I.
PARTS = $(PROG) testsuite common $(foreach IMPL,$(IMPLS),$(IMPL_PATH)/$(IMPL))
OBJS = $(foreach OBJ,$(PARTS),$(OBJ).o)


all: $(PROG)

/%.o: %.c
	$(CCLD) $(CFLAGS) -c $<

$(PROG): $(OBJS)
	$(CCLD) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJS) $(PROG) *.log

.PHONY: all clean