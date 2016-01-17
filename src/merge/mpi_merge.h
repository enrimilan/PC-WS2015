#ifndef MPI_MERGE_H
#define MPI_MERGE_H

#include "common.h"
#include "mpi.h"

double mpi_merge_master(data_t *A, int lenA, data_t *B, int lenB, data_t *result);
void mpi_merge_slave(int lenA, int lenB);
int get_length_of_local_array(int len);
void distribute_data(data_t *A, data_t *B, int lenA, int lenB, MPI_Win *win_A, MPI_Win *win_B, long *global_index_start);
void mpi_merge(int len_A, int len_B, long global_index_start, long global_index_end, MPI_Win win_A, MPI_Win win_B, data_t *result);
void calculate_coranks(long index, MPI_Win win_A, long lenA, long *corankA, MPI_Win win_B, long lenB, long *corankB);
data_t get_data(data_t *chunk, long start, long end, long len, MPI_Win win);

#endif
