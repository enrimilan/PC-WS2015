#include "common.h"
#include <sys/time.h>


double getTimestamp(void) {
	struct timeval now;
	gettimeofday(&now, NULL);
	
	return (double) ((long long) now.tv_sec*1000000L + (long long) now.tv_usec);
}


void coranks(int index, data_t *A, int lenA, int *corankA, data_t *B, int lenB, int *corankB) {
	int minA = (index > lenA) ? index-lenA : 0;
	int minB = 0;

	// Initialize coranks
	*corankA = (lenA < index) ? lenA : index;
	*corankB = index - *corankA;

	// Check if coranks are correct and do binary search if they are not
	while (1) {
		if (*corankA > 0 && *corankB < lenA && A[*corankA-1] > B[*corankB]) {
			int corr = (*corankA - minA + 1)/2;
			minB = *corankB;
			*corankA -= corr;
			*corankB += corr;
		}
		else if (*corankA < lenB && *corankB > 0 && B[*corankB-1] >= A[*corankA]) {
			int corr = (*corankB - minB + 1)/2;
			minA = *corankA;
			*corankA += corr;
			*corankB -= corr;
		}
		else break;
	}
}