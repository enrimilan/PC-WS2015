#ifndef COMMON_H
#define COMMOM_H

#include <stdlib.h>


/** Type definitions */

typedef long data_t;	// Type of data to be merged


/** Prototypes */

double getTimestamp(void);
void coranks(int index, data_t *A, int lenA, int *corankA, data_t *B, int lenB, int *corankB);


#endif