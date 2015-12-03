#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>


/** Prototypes */

static void hello(int taskno);


/** Implementations */

int main(int argc, char** argv) {
	printf("Total number of threads: %d\n\n", __cilkrts_get_nworkers());

	for (int i=1; i<=100; i++) {
		cilk_spawn hello(i);
	}
	cilk_sync;
	
	return EXIT_SUCCESS;
}


static void hello(int taskno) {
	printf("This is task %d\n", taskno);
}