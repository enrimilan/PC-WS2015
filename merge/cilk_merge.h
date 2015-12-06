#ifndef CILK_MERGE_H
#define CILK_MERGE_H

#include "common.h"

double cilk_merge(data_t *A, int A_len, data_t *B, int B_len, data_t *result);

#endif