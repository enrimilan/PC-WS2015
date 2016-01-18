#include <stdlib.h>
#include <mpi.h>
#include "test/tester.h"
#include "merge/mpi_merge.h"

#define 	MASTER_PROC_RANK	(0)


int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	int threads = parseArguments(argc, argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == MASTER_PROC_RANK) {
		// Execute tests as master
		Implementation impl = {"MPI", mpi_merge_master};
		executeTestcases(&impl, threads);
	}
	else {
		// Execute tests as slave
		executeSlave(mpi_merge_slave, threads);
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}