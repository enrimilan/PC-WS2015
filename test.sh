#!/bin/sh

SRC_DIR="src"
PLOT_DIR="plots"

SERVERS="saturn jupiter"


bash cleanup.sh

for server in $SERVERS
do
	exec_script="test_"$server".sh"

	# Upload sources
	echo -n "Uploading sources to "$server"... "
	ssh $server rm -rf *
	scp -r output src $exec_script $server: > /dev/null
	echo "Done!"

	# Execute tests
	ssh $server bash $exec_script

	# Download benchmarks
	echo -en "\nDownloading benchmarks from "$server"... "
	scp $server:output/*.dat ./output > /dev/null
	echo "Done!"
done

# Draw plots
cd $PLOT_DIR
echo -n "Drawing plots... "
gnuplot PerfPlots > /dev/null 2> /dev/null
echo "Done!"