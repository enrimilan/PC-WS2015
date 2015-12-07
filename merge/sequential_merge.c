#include "sequential_merge.h"


double sequential_merge(data_t *A, int A_len, data_t *B, int B_len, data_t *result) {
	if (result == NULL)
		return 0;

	usecs start = getTimestamp();

	int i = 0;
	int j = 0;
	int k = 0;

	while(i<A_len && j<B_len){
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

	while(i<A_len){
		result[k] = A[i];
		i++;
		k++;
	}

	while(j<B_len){
		result[k] = B[j];
		j++;
		k++;
	}
	
	usecs end = getTimestamp();
	
	return timeDiff(start, end);
}
