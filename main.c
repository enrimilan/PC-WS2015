#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "testsuite.h"


/** Constants */

// Comment the following line out to prevent that information on failed test cases is printed into the error log	
//#define		PRINT_ERRORS

// How many times shall we repeat a testcase
#define		REPEAT_TIMES	(50)

// Name of the log containing failed test cases			
#define		ERROR_LOG		"error.log"

// Name of the log containing execution time statistics	
#define		STAT_LOG		"stats.log"		


/** Type definitions */

typedef enum {FALSE, TRUE} Bool;

typedef struct {
	int id;
	
	data_t *A;
	int A_len;
	
	data_t *B;
	int B_len;
	
} Testcase;

typedef struct {
	int times;
	double t_min;
	double t_max;
	double t_avg;

} ExecutionStatistic;

typedef struct {
	data_t* merged;
	ExecutionStatistic stats;

} Result;


/** Prototypes */

static void executeTestcases(void);
static void executeSingleTest(Testcase test);
static void testImplementation(Testcase test, Implementation impl, Result refResult);
static Result executeMerge(Testcase test, Implementation impl);
static Testcase generateTestcaseFromDefinition(TestcaseDefinition def);
static Bool verifyResult(data_t* result, data_t* reference, int len);
static void updateExecutionStats(ExecutionStatistic *stats, double execTime);

static data_t* allocArray(int len);
static data_t* fillArray(data_t start, data_t inc, int len);
static void freeArray(data_t *arr);
static void cleanup(void);

static void printArray(data_t* arr, int len);
static void printExecutionStats(char *implName, ExecutionStatistic stats);
static void printStatComparison(char *implName, ExecutionStatistic implStats, ExecutionStatistic refStats);
static void printFailedTestInfo(data_t* result, data_t* reference, int len);
static void printErrorStats(FILE* dest);


/** Global variables */

static int failedTests = 0;			
static FILE* errorLog = NULL;		
static FILE* statLog = NULL;


/** Implementations */

int main(void) {
	// Open logs
	errorLog = fopen(ERROR_LOG, "w");
	statLog = fopen(STAT_LOG, "w");

	if (errorLog == NULL || statLog == NULL) {
		fprintf(stderr, "Error: Could not open logs.");
		return EXIT_FAILURE;
	}
		
	// Execute testcases
	executeTestcases();
	cleanup();
	
	return EXIT_SUCCESS;
}


static void executeTestcases(void) {	
	for (int i=0; i<numberOfTests; i++) {
		Testcase test = generateTestcaseFromDefinition(testcases[i]);
		fprintf(statLog, "TESTCASE %d\n______________\n", test.id);
		executeSingleTest(test);
		fprintf(statLog, "\n\n\n");
	}

	printErrorStats(errorLog);
	printErrorStats(stdout);	
}


static void executeSingleTest(Testcase test) {
	// Get reference solution
	Result refResult = executeMerge(test, refImpl);
	printExecutionStats(refImpl.name, refResult.stats);

	// Test each parallel implementation against reference solution
	for (int i=0; i<numberOfImpl; i++)
		testImplementation(test, impl[i], refResult);

	// Free memory
	freeArray(test.A);
	freeArray(test.B);
	freeArray(refResult.merged);
}


static void testImplementation(Testcase test, Implementation impl, Result refResult) {
	Result implResult = executeMerge(test, impl);

	// Verify that parallel implementation merged correctly
	int merged_len = test.A_len + test.B_len;

	if (!verifyResult(implResult.merged, refResult.merged, merged_len)) {
		failedTests++;
		fprintf(errorLog, "TESTCASE %d (%s) failed!\n", test.id, impl.name);

		#ifdef PRINT_ERRORS
			printFailedTestInfo(implResult.merged, refResult.merged, merged_len);
		#endif
	}

	printStatComparison(impl.name, implResult.stats, refResult.stats);
	freeArray(implResult.merged);
}


static Result executeMerge(Testcase test, Implementation impl) {
	data_t* merged = allocArray(test.A_len + test.B_len);
	ExecutionStatistic stats = {};
	stats.t_min = DBL_MAX;
	
	for (int i=1; i<=REPEAT_TIMES; i++) {
		double execTime = impl.func(test.A, test.A_len, test.B, test.B_len, merged);
		updateExecutionStats(&stats, execTime);
	}

	Result res = {merged, stats};
	return res;
}


static Testcase generateTestcaseFromDefinition(TestcaseDefinition def) {
	Testcase test;
	test.id = def.id;
	
	test.A = fillArray(def.A_start, def.A_inc, def.A_len);
	test.A_len = def.A_len;
	
	test.B = fillArray(def.B_start, def.B_inc, def.B_len);
	test.B_len = def.B_len;

	return test;
}


static Bool verifyResult(data_t* result, data_t* reference, int len) {
	for (int i=0; i<len; i++) {
		if (result[i] != reference[i])
			return FALSE;
	}

	return TRUE;
}


static void updateExecutionStats(ExecutionStatistic *stats, double execTime) {
	if (stats->t_min > execTime)
		stats->t_min = execTime;
		
	if (stats->t_max < execTime)
		stats->t_max = execTime;

	stats->t_avg = (stats->t_avg*stats->times + execTime) / (stats->times+1);
	stats->times++;
}


static data_t* allocArray(int len) {
	if (len <= 0)
		return NULL;
	
	return malloc(sizeof(data_t)*len);
}


static data_t* fillArray(data_t start, data_t inc, int len) {
	if (inc <= 0)
		return NULL;
		
	data_t *arr = allocArray(len);

	// Fill array
	for (int i=0; i<len; i++)
		arr[i] = start + inc*i;

	return arr;
}


static void freeArray(data_t *arr) {
	if (arr != NULL)
		free(arr);
}


static void cleanup(void) {
	if (errorLog != NULL)
		fclose(errorLog);

	if (statLog != NULL)
		fclose(statLog);
}


/** Printing functions */

static void printArray(data_t* arr, int len) {
	fprintf(errorLog, "[");

	for (int i=0; i<len; i++) {
		fprintf(errorLog, " %ld", arr[i]);
	}

	fprintf(errorLog, " ]\n");
}


static void printExecutionStats(char *implName, ExecutionStatistic stats) {
	fprintf(statLog, "\n%s:\n", implName);
	fprintf(statLog, "T_min = %llu µs\nT_avg = %llu µs\nT_max = %llu µs\n",
		(usecs)stats.t_min, (usecs)stats.t_avg, (usecs)stats.t_max);
}


static void printStatComparison(char *implName, ExecutionStatistic implStats, ExecutionStatistic refStats) {
	printExecutionStats(implName, implStats);

	// Print speedup
	double speedup = 0;
	if (implStats.t_avg != 0)
		speedup = refStats.t_avg / implStats.t_avg;
	fprintf(statLog, "Speedup = %f\n", speedup);
}


static void printFailedTestInfo(data_t* result, data_t* reference, int len) {
	fprintf(errorLog, "Expected: ");
	printArray(reference, len);

	fprintf(errorLog, "Result:   ");
	printArray(result, len);

	for (int i=0; i<len+7; i++)
		fprintf(errorLog, "__");
	fprintf(errorLog, "\n\n\n");
}


static void printErrorStats(FILE* dest) {
	int passedTests = numberOfTests*numberOfImpl - failedTests;
	
	fprintf(dest, "\n%d test%s passed, %d test%s failed!\n",
		passedTests, (passedTests == 1) ? "" : "s",
		failedTests, (failedTests == 1) ? "" : "s");
}