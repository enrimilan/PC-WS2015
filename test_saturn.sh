#!/bin/sh

SRC_DIR="src"


# Build sources
cd $SRC_DIR
echo -n "Saturn: Building sources... "
make cilk omp > /dev/null 2> /dev/null
echo "Done!"

# Execute Cilk
echo -e "Saturn: Executing Cilk...\n"
for threads in 1 2 {4..48..4}
do
	./cilk $threads 2> /dev/null
done

# Execute OpenMP
echo -e "\nSaturn: Executing OpenMP...\n"
for threads in 1 2 {4..48..4}
do
	./omp $threads 2> /dev/null
done