# Makefile for the test programs
# Date: 07.01.2016


PROGS = cilk omp mpi
OUTPUT_DIR = ../output
IMPL_PATH = merge/$(1)_merge

CILK_CC = gcc
OMP_CC = gcc
MPI_CC = mpicc

CFLAGS = -Wall -O3 -g -std=c99 -I.
CILK_FLAGS = $(CFLAGS) -fcilkplus
OMP_FLAGS = $(CFLAGS) -fopenmp
MPI_FLAGS = $(CFLAGS)

FILES = $(1) $(call IMPL_PATH,$(1)) $(call IMPL_PATH,sequential) test/tester test/testsuite common
SOURCES = $(foreach SRC,$(call FILES,$(1)),$(SRC).c)


all: $(PROGS)

cilk: 
	$(CILK_CC) $(CILK_FLAGS) $(call SOURCES,$@) -o $@

omp: 
	$(OMP_CC) $(OMP_FLAGS) $(call SOURCES,$@) -o $@

mpi: 
	$(MPI_CC) $(MPI_FLAGS) $(call SOURCES,$@) -o $@

clean:
	rm -f $(PROGS) $(OUTPUT_DIR)/*.log $(OUTPUT_DIR)/*.dat

.PHONY: all clean