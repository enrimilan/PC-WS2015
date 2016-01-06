#include "testsuite.h"
#include "merge/cilk_merge.h"
#include "merge/omp_merge.h"
#include "merge/mpi_merge.h"
#include "merge/sequential_merge.h"
#include <limits.h>


/** Definition of test cases */
TestcaseDefinition testcases[] = {
	{"Interleaved",		1,			2,		2,	2},
	{"Disjunct", 		LONG_MIN,	1,		1,	1}
};

TestSize sizes[] = {
	100,
	1000,
	10000,
	100000,
	1000000
};


/** The sequential reference implementation */
Implementation refImpl = {"Sequential", sequential_merge};


/** The parallel implementations to be tested */
Implementation impl[] = {
	{"Cilk", cilk_merge},
	{"OpenMP", omp_merge}
	//{"MPI", mpi_merge}
};


int numberOfTests = sizeof(testcases)/sizeof(TestcaseDefinition);
int numberOfImpl = sizeof(impl)/sizeof(Implementation);
int numberOfSizes = sizeof(sizes)/sizeof(TestSize);