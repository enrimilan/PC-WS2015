#include <stdlib.h>
#include <cilk/cilk_api.h>
#include "test/tester.h"
#include "merge/cilk_merge.h"


int main(int argc, char** argv) {
	int threads = parseArguments(argc, argv);
	
	// Set number of threads  
	__cilkrts_set_param("nworkers", argv[1]);

	// Execute tests
	Implementation impl = {"Cilk", cilk_merge};
	executeTestcases(&impl, threads);
	
	return EXIT_SUCCESS;
}