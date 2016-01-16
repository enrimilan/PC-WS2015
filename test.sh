#!/bin/sh

SRC_DIR="src"
PLOT_DIR="plots"

SERVERS="saturn"


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
	echo -n "Downloading benchmarks from "$server"... "
	scp -r $server:output . > /dev/null
	echo "Done!"
done


# Draw plots
cd $PLOT_DIR
echo -n "Drawing plots... "
gnuplot PerfPlots > /dev/null
echo "Done!"