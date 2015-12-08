#include "sequential_merge.h"


double sequential_merge(data_t *A, int lenA, data_t *B, int lenB, data_t *result) {
	if (result == NULL)
		return 0;

	double start = getTimestamp();

	int i = 0;
	int j = 0;
	int k = 0;

	while(i<lenA && j<lenB){
		if(A[i]<B[j]){
			result[k] = A[i];
			i++;
		}
		else{
			result[k] = B[j];
			j++;
		}
		k++;
	}

	while(i<lenA){
		result[k] = A[i];
		i++;
		k++;
	}

	while(j<lenB){
		result[k] = B[j];
		j++;
		k++;
	}
	
	double end = getTimestamp();
	
	return end-start;
}
