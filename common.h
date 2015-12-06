#ifndef COMMON_H
#define COMMOM_H

#include <stdlib.h>


/** Type definitions */

typedef long data_t;	// Type of data to be merged

typedef unsigned long long usecs;


/** Prototypes */

usecs getTimestamp(void);
double timeDiff(usecs t1, usecs t2);


#endif