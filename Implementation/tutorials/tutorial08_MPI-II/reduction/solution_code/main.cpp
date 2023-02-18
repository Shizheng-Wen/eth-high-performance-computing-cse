#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <mpi.h>
#include <assert.h>

inline long exact(const long N){
    return N*(N+1)/2;
}

long reduce_mpi(const int rank, long mysum){
    long sum;
    /* int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm) */
    MPI_Reduce(&mysum, &sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD); //collect partial results from each rank to rank 0.
    return sum;
}

long reduce_manual(int rank, int size, long sum){
    // Size is a power of 2 for simplicity
    const int TAG = 1337;
    for(int send_rec_border = size/2; send_rec_border >= 1; send_rec_border/=2)
    {
        if(rank < send_rec_border)
        {
            long buffer;
            MPI_Recv(&buffer, 1, MPI_LONG, rank+send_rec_border, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += buffer;
        } else if(rank <= send_rec_border*2)
        {
            MPI_Send(&sum, 1, MPI_LONG, rank-send_rec_border, TAG, MPI_COMM_WORLD);
        }
    }
    return sum;
}


int main(int argc, char** argv){
    const long N = 1000000;


    //-------------------------
    // Initialize MPI
    //-------------------------
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    //-------------------------
    // Partial sums
    //-------------------------
    // Determine work load per rank
    long N_per_rank = N / size;

    // Remark: Start summation at 1!!! (1+2+3+...+N)
    long N_start = rank * N_per_rank + 1;
    long N_end = (rank+1) * N_per_rank;
    // the last rank has to do a few additions more if size does not divide N
    if(rank == size-1){
        N_end += N % size;
    }

    // Compute partial in each rank: N_start + (N_start+1) + ... + (N_start+N_per_rank-1)
    long sum = 0;
    for(long i = N_start; i <= N_end; ++i){
        sum += i;
    }


    //-------------------------
    // Reduction
    //-------------------------

    // Case 1: naive reduction
    long sum_mpi_v1 = reduce_mpi(rank, sum);
    if(rank == 0){
        std::cout << std::left << std::setw(30) << "Final result (exact): " << exact(N) << std::endl;
        std::cout << std::left << std::setw(30) << "Final result (MPI:naive): " << sum_mpi_v1 << std::endl;
    }

    // Case 2: distributed reduction
    long sum_mpi_v2 = reduce_manual(rank, size, sum);
    if(rank == 0){
        std::cout << std::left << std::setw(30) << "Final result (MPI:manual): " << sum_mpi_v2 << std::endl;
    }


    MPI_Finalize();

    return 0;
}
