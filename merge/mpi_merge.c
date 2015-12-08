#include "mpi_merge.h"


double mpi_merge(data_t *A, int lenA, data_t *B, int lenB, data_t *result) {
	if (result == NULL)
		return 0;

	double start = getTimestamp();

	// TODO: implement!
	
	double end = getTimestamp();
	return end-start;
}