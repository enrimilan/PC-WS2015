#ifndef TESTSUITE_H
#define TESTSUITE_H

#include "common.h"


/** Type definitions */

typedef int TestSize;

typedef struct {
	char name[25];
	
	data_t A_start;
	data_t A_inc;
	data_t B_start;
	data_t B_inc;
	 
} TestcaseDefinition;

typedef struct {
	char* name;
	double (*func) (data_t*, int, data_t*, int, data_t*);
	
} Implementation;


/** Global variables */

extern TestcaseDefinition testcases[];
extern TestSize sizes[];
extern Implementation refImpl;
extern int numberOfTests;
extern int numberOfSizes;

#endif