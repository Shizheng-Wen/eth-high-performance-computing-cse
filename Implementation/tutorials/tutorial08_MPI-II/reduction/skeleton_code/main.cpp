#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <mpi.h>

inline long exact(const long N){
    return N*(N+1)/2.0;
}

long reduce_mpi(const int rank, long& mysum){
    // TODO: Perform the reduction using blocking collectives.
    double sum;
    MPI_Reduce(&mysum,&sum,1,MPI_LONG,MPI_SUM,0,MPI_COMM_WORLD)
    return sum;
}

void reduce_manual(int rank, int size, long& sum){
    // TODO: Implement a tree based reduction using blocking point-to-point communication.
    // Size is a power of 2 for simplicity

    const int TAG = 1337；

    for(int send_rec_border = size/2; send_rec_border >= 1; send_rec_border/=2)
    {
        if(rank < send_rec_border)
        {
            long buffer;
            MPI_Recv(&buffer, 1, MPI_LONG, rank+send_rec_border, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += buffer;
        } 
        else if(rank <= send_rec_border*2){
            MPI_Send(&sum, 1, MPI_LONG, rank-send_rec_border, TAG, MPI_COMM_WORLD);
        }
    }
}


int main(int argc, char** argv){

    const long N = 1000000;

    MPI_Init(&argc,&argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    // workload per rank
    long N_per_rank = N/size;

    // compute partial summarization
    long N_start = rank * N_per_rank + 1;
    long N_end = (rank + 1) * N_per_rank；
    // the last element added into the last rank
    if (ranke == size - 1){
        N_end += N % size;
    }

    // local sum
    long sum = 0;
    for (long i=N_start; i <= N_per_rank; ++i){
        sum += i;
    }

    // case 1: naive reduction
    long globalsum = reduce_mpi(rank,sum)

    // case 2: distributed reduction
    long globalsum = reduce_manual(rank,size,sum)
    


    long sum = 0;

    sum = exact(N);

    std::cout << std::left << std::setw(25) << "Final result (exact): " << sum << std::endl;

    return 0;
    MPI_Finalize();
}
