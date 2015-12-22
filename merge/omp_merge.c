#include "omp_merge.h"
#include "sequential_merge.h"
#include "omp.h"

double omp_merge(data_t *A, int lenA, data_t *B, int lenB, data_t *result) {
	if (result == NULL)
		return 0;

	//usually some multiple of the number of cores	
	int p = omp_get_max_threads();
	double start1 = getTimestamp();
	#pragma omp parallel
	{
 		#pragma omp for
 		for (int i=0; i<p; i++) {
 			int startA, endA, startB, endB;
 			long start = (long)i*(long)(lenA+lenB)/(long)p;
 			long end = (long)(i+1)*(long)(lenA+lenB)/(long)p;
			coranks(start, A, lenA, &startA, B, lenB, &startB);
			coranks(end, A, lenA, &endA, B, lenB, &endB);
			(void) sequential_merge(A+startA, endA-startA, B+startB, endB-startB, result+start);
 		}
	}
	double end = getTimestamp();
	
	return end-start1;
}
