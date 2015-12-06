#include "cilk_merge.h"


double cilk_merge(data_t *A, int A_len, data_t *B, int B_len, data_t *result) {
	if (result == NULL)
		return 0;

	usecs start = getTimestamp();

	// TODO: implement!
	
	usecs end = getTimestamp();
	return timeDiff(start, end);
}