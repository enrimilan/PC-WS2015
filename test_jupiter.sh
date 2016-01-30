#!/bin/sh

SRC_DIR="src"


# Build sources
cd $SRC_DIR
echo -n "Jupiter: Building sources... "
make mpi > /dev/null 2> /dev/null
echo "Done!"

echo -e "Jupiter: Executing MPI...\n"
for procs in 1 16 {48..576..48}
do
	# Compute processes per host
	arg_string=""
	for host in {0..35}
	do
		procs_per_host=$(((procs - host*16) > 16 ? 16 : procs - host*16))
		
		if (("$procs_per_host" > 0))
		then
			arg_string=$arg_string" -host jupiter"$host" -np "$procs_per_host
		fi
	done

	# Execute MPI
	mpirun $arg_string ./mpi $procs 2> /dev/null
done