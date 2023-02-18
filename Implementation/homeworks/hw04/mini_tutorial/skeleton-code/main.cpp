#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

MPI_Status status;

void print_array(float* x, int n){
	printf("[");
	for(int i = 0; i < n-1; ++i){
		printf("%.3f, ", x[i]);
	}
	printf("%.3f]", x[n-1]);
}


int main(int argc, char *argv[])
{
	// TODO: Start-up the MPI environment and determine this process' rank ID as
    // well as the total number of processes (=ranks) involved in the
    // communicator
	int rank, procs;

	int n = 5; // size of the array

	float* x = (float*) calloc(n, sizeof(float));

	if(rank == 0){
		for(int i = 0; i < n; ++i){
			x[i] = 1.1*i;
		}
	}

	
	if(rank == 0) printf("\n==============================\n");
	for(int i = 0; i < procs; ++i){
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank == i){
			printf("Hello BEFORE the message passing! I am rank==%i and I have the array: ", i);
			print_array(x,n);
			printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0) printf("==============================\n\n");
	

	double t0 = MPI_Wtime(); 

	// TODO: master rank sending its array to rank == 1
	
	double t1 = MPI_Wtime();

	if(rank == 0) printf("send/recv execution time: %.20f", t1 - t0);

	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0) printf("\n==============================\n");
	
	for(int i = 0; i < procs; ++i){
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank == i){
			printf("Hello AFTER the message passing! I am rank==%i and I have the array: ", i);
			print_array(x,n);
			printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0) printf("==============================\n\n");

	// TODO: Shutdown the MPI environment

	return 0;
}
