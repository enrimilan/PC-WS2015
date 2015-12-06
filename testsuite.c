#include "testsuite.h"
#include "merge/cilk_merge.h"
#include "merge/omp_merge.h"
#include "merge/mpi_merge.h"
#include "merge/sequential_merge.h"


/** Definition of test cases */
TestcaseDefinition testcases[] = {
	{1,		1,2,1000000,		2,2,1000000},
	{2, 	-100000,1,100000,	1,1,100000}
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