#include <stdlib.h>
#include <omp.h>
#include "test/tester.h"
#include "merge/omp_merge.h"


int main(int argc, char** argv) {
	int threads = parseArguments(argc, argv);
	
	// Set number of threads  
	omp_set_dynamic(0);
	omp_set_num_threads(threads);

	// Execute tests
	Implementation impl = {"OMP", omp_merge};
	testImplementation(impl, threads);
	
	return EXIT_SUCCESS;
}