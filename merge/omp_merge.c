#include "omp_merge.h"
#include "sequential_merge.h"
#include "omp.h"

/** Prototypes */

void omp_merge_part(int start, int end, data_t *A, int lenA, data_t *B, int lenB, data_t *result);

/** Implementations */

double omp_merge(data_t *A, int lenA, data_t *B, int lenB, data_t *result) {
	if (result == NULL)
		return 0;

	double start = getTimestamp();
	omp_merge_part(0, lenA+lenB, A, lenA, B, lenB, result);
	double end = getTimestamp();
	
	return end-start;
}

void omp_merge_part(int start, int end, data_t *A, int lenA, data_t *B, int lenB, data_t *result){

	//get number of available threads
	int n = omp_get_max_threads() - omp_get_num_threads();
	if(n<2){
		int startA, endA, startB, endB;
		coranks(start, A, lenA, &startA, B, lenB, &startB);
		coranks(end, A, lenA, &endA, B, lenB, &endB);
		(void) sequential_merge(A+startA, endA-startA, B+startB, endB-startB, result+start);
	}
	else{
		//distribute the work across threads
		int middle = start + (end-start)/2;
		#pragma omp parallel sections 
		{

			#pragma omp section
			omp_merge_part(start, middle, A, lenA, B, lenB, result);

			#pragma omp section
			omp_merge_part(middle, end, A, lenA, B, lenB, result);

		}
		
	}
}
