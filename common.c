#include "common.h"
#include <sys/time.h>


usecs getTimestamp(void) {
	struct timeval now;
	gettimeofday(&now, NULL);
	
	return (usecs)(now.tv_sec*100000L + now.tv_usec);
}


double timeDiff(usecs t1, usecs t2) {
	if (t1 < t2)
		return (double)(t2-t1);

	return (double)(t1-t2);
}