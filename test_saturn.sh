#!/bin/sh

SRC_DIR="src"


# Build sources
cd $SRC_DIR
echo -n "Building sources... "
make all > /dev/null 2> /dev/null
echo "Done!"


# Execute implementations
echo -e "Executing implementations...\n"
for threads in {1..48}
do
	echo -n $threads" thread"
	[ "$threads" -eq 1 ] && echo ":" || echo "s:" 
	
	./cilk $threads 2> /dev/null
	./omp $threads 2> /dev/null
	mpirun -np $threads ./mpi $threads 2> /dev/null

	echo
done