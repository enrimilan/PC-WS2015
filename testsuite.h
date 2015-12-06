#ifndef TESTSUITE_H
#define TESTSUITE_H

#include "common.h"


/** Type definitions */

typedef double (*MergeFunc) (data_t*, int, data_t*, int, data_t*);

typedef struct {
	int id;
	
	data_t A_start;
	data_t A_inc;
	int A_len;

	data_t B_start;
	data_t B_inc;
	int B_len;
	 
} TestcaseDefinition;

typedef struct {
	char* name;
	MergeFunc func;
	
} Implementation;


/** Global variables */

extern TestcaseDefinition testcases[];
extern Implementation refImpl;
extern Implementation impl[];
extern int numberOfTests;
extern int numberOfImpl;


#endif