#ifndef MPI_MERGE_H
#define MPI_MERGE_H

#include "common.h"

double mpi_merge_master(data_t *A, int lenA, data_t *B, int lenB, data_t *result);
void mpi_merge_slave(int lenA, int lenB);

#endif