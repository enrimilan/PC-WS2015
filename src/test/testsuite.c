#include "testsuite.h"
#include "merge/sequential_merge.h"
#include <limits.h>


/** Definition of test cases */
TestcaseDefinition testcases[] = {
	{"Interleaved",		1,			2,		2,	2},
	{"Disjunct", 		LONG_MIN,	1,		1,	1}
};

TestSize sizes[] = {
	1000,
	10000,
	50000,
	100000,
	250000,
	500000
};


/** The sequential reference implementation */
Implementation refImpl = {"Sequential", sequential_merge};


int numberOfTests = sizeof(testcases)/sizeof(TestcaseDefinition);
int numberOfSizes = sizeof(sizes)/sizeof(TestSize);