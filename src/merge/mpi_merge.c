#include "mpi_merge.h"
#include <mpi.h>
#include <stdio.h>
#include "sequential_merge.h"

#define ROOT (0)

static int world_size;

double mpi_merge_master(data_t *A, int lenA, data_t *B, int lenB, data_t *result) {
    
	// Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    long local_array_A_length = get_length_of_local_array(lenA);
    long local_array_B_length = get_length_of_local_array(lenB);
	
	MPI_Win win_A, win_B;
	long global_index_start;
	
	distribute_data(A, B, lenA, lenB, &win_A, &win_B, &global_index_start);
	long global_index_end = global_index_start + local_array_A_length + local_array_B_length;
	
	double start = MPI_Wtime() * 1000000;
	data_t *local_merged_array = malloc((local_array_A_length + local_array_B_length) * sizeof(data_t));
	mpi_merge(lenA, lenB, global_index_start, global_index_end, win_A, win_B, local_merged_array);
	double end = MPI_Wtime() * 1000000;
	
	MPI_Gather(local_merged_array, local_array_A_length + local_array_B_length, MPI_LONG, result, local_array_A_length + local_array_B_length, MPI_LONG, ROOT, MPI_COMM_WORLD);
	
	MPI_Win_free(&win_A);
	MPI_Win_free(&win_B);
	
	return end - start;
}


void mpi_merge_slave(int lenA, int lenB) {
    
	// Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	long local_array_A_length = get_length_of_local_array(lenA);
    long local_array_B_length = get_length_of_local_array(lenB);

	MPI_Win win_A, win_B;
	long global_index_start;
	distribute_data(NULL, NULL, lenA, lenB, &win_A, &win_B, &global_index_start);
	long global_index_end = global_index_start + local_array_A_length + local_array_B_length;
	
	data_t *local_merged_array = malloc((local_array_A_length + local_array_B_length) * sizeof(data_t));
	mpi_merge(lenA, lenB, global_index_start, global_index_end, win_A, win_B, local_merged_array);
	MPI_Gather(local_merged_array, local_array_A_length + local_array_B_length, MPI_LONG, NULL, 0, MPI_LONG, ROOT, MPI_COMM_WORLD);

	MPI_Win_free(&win_A);
	MPI_Win_free(&win_B);
}

int get_length_of_local_array(int len){
	long length = 1;
	if(len > world_size){
		length = len / world_size;
	}
	return length;
}

void distribute_data(data_t *A, data_t *B, int lenA, int lenB, MPI_Win *win_A, MPI_Win *win_B, long *global_index_start){
	

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    long local_array_A_length = get_length_of_local_array(lenA);
    long local_array_B_length = get_length_of_local_array(lenB);
	
	long displacement = 0;

	//first array
	int displs_A[world_size];
	data_t *localdata_A;
	int sendcounts_A[world_size];
	for(int i = 0; i<world_size; i++){
		sendcounts_A[i] = local_array_A_length;
		displs_A[i] = displacement;
		displacement = displacement + sendcounts_A[i];
	}
	localdata_A = malloc(local_array_A_length * sizeof(data_t));
	MPI_Scatterv(A, sendcounts_A, displs_A, MPI_LONG, localdata_A, local_array_A_length, MPI_LONG, ROOT, MPI_COMM_WORLD);

	//sedond array
	displacement = 0;
	int displs_B[world_size];
	data_t *localdata_B;
	int sendcounts_B[world_size];
	for(int i = 0; i<world_size; i++){
		sendcounts_B[i] = local_array_B_length;
		displs_B[i] = displacement;
		displacement = displacement + sendcounts_B[i];
	}
	localdata_B = malloc(local_array_B_length * sizeof(data_t));
	MPI_Scatterv(B, sendcounts_B, displs_B, MPI_LONG, localdata_B, local_array_B_length, MPI_LONG, ROOT, MPI_COMM_WORLD);

	//wait until distribution has finished...
	MPI_Barrier(MPI_COMM_WORLD);
	
	//expose the data
	MPI_Win_create(localdata_A, local_array_A_length, sizeof(data_t), MPI_INFO_NULL, MPI_COMM_WORLD, win_A);
	MPI_Win_create(localdata_B, local_array_B_length, sizeof(data_t), MPI_INFO_NULL, MPI_COMM_WORLD, win_B);

	*global_index_start = displs_A[world_rank] + displs_B[world_rank];
}

void mpi_merge(int len_A, int len_B, long global_index_start, long global_index_end, MPI_Win win_A, MPI_Win win_B, data_t *result){
	//step 1: calculate co-ranks
	
	long corank_A_start, corank_B_start, corank_A_end, corank_B_end;
	calculate_coranks(global_index_start, win_A, len_A, &corank_A_start, win_B, len_B, &corank_B_start);
	calculate_coranks(global_index_end, win_A, len_A, &corank_A_end, win_B, len_B, &corank_B_end);

	//step 2: get the data we need to merge
	long chunkA_size = (corank_A_end-1 - corank_A_start + 1)>0 ? (corank_A_end-1 - corank_A_start + 1) : 0;
	data_t *chunkA = NULL;
	if(chunkA_size>0){
		chunkA = malloc(chunkA_size*sizeof(data_t));
		get_data(chunkA, corank_A_start, corank_A_end-1, len_A, win_A);
	}

	long chunkB_size = (corank_B_end-1 - corank_B_start + 1)>0 ? (corank_B_end-1 - corank_B_start + 1) : 0;
	data_t *chunkB = NULL;
	if(chunkB_size>0){
		chunkB = malloc(chunkB_size*sizeof(data_t));
		get_data(chunkB, corank_B_start, corank_B_end-1, len_B, win_B);
	}
	
	//step 3: sequential merge
	sequential_merge(chunkA, chunkA_size, chunkB, chunkB_size, result);
	
	//wait until ALL processes finished work...
	MPI_Barrier(MPI_COMM_WORLD);
}

void calculate_coranks(long index, MPI_Win win_A, long lenA, long *corankA, MPI_Win win_B, long lenB, long *corankB){
	long j = (lenA < index) ? lenA : index;
	long k = index - j;
	long j_low = ((index - lenB) > 0 ) ? (index - lenB) : 0;
	long k_low=0, delta;
	long chunk[1];
	do {
		if (j>0 && k<lenB && get_data(chunk, j-1, j-1, lenA, win_A)>get_data(chunk, k, k, lenB, win_B)){
			delta = (1 + j - j_low) / 2;
			k_low = k;
			j = j - delta; 
			k = k + delta;
		}
		else if (k>0 && j<lenA && get_data(chunk, k-1, k-1, lenB, win_B)>=get_data(chunk, j, j, lenA, win_A)) {
			delta = (1+k-k_low)/2;
			j_low = j;
			k = k - delta; 
			j = j + delta;
		}
		else {
			//found co-ranks
			*corankA = j;
			*corankB = k;
			break;
		} 
	}while(1);
}

data_t get_data(data_t *chunk, long start, long end, long len, MPI_Win win){
	long elements_per_process = len / world_size;
	int target_rank = start / elements_per_process;
	long local_index = start - (target_rank * elements_per_process);
	long count = end - start + 1;
	long chunk_position = 0;
	long remaining;
	while(count>0){
		remaining = elements_per_process - local_index;
		long to_copy = remaining > count ? count : remaining;
		MPI_Win_lock(MPI_LOCK_SHARED, target_rank, MPI_MODE_NOCHECK, win);
		MPI_Get(chunk+chunk_position, to_copy, MPI_LONG, target_rank, local_index, to_copy, MPI_LONG, win);
		MPI_Win_unlock(target_rank, win);
		count = count - remaining;
		local_index = 0;
		target_rank++;
		chunk_position = chunk_position + remaining;
	}
	return chunk[0];
}
