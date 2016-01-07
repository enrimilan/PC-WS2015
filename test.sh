#!/bin/sh

SRC_DIR="src"
PLOT_DIR="plots"


# Compile sources
cd $SRC_DIR
echo -n "Compiling sources... "
make clean > /dev/null 2> /dev/null
make all > /dev/null 2> /dev/null
echo -e "Done!"


# Execute Cilk
echo -e "\nExecuting Cilk..."
for threads in {1..48..2}
do
	./cilk $threads
done


# Execute OpenMP
echo -e "\nExecuting OpenMP..."
for threads in {1..48..2}
do
	./omp $threads
done
cd ..


# Draw plots
cd $PLOT_DIR
echo -en "\nDrawing plots... "
gnuplot PerfPlots > /dev/null 2> /dev/null
echo -e "Done!\n"
cd ..
