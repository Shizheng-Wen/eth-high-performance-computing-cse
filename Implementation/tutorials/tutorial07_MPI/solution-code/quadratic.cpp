#include <iostream>
#include <mpi.h>

//correct result is 39159.4

int main(int argc, char** argv)
{
    MPI_Init(&argc,&argv);

    const int n = 32768;

    /*Strategy:
    To compute 
    
      sum(i=0...n-1)sum(j=0...n-1) v_i A_ij w_j
    
    we will distribute the "i" indices to the MPI ranks.
    Each rank will own n/size of the total of n elements in the i-direction.
    */

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    //assume n is divisible by size
    if (n%size !=0)
    {
        std::cerr << "n = " << n << " must be divisible by the number of ranks.\n";
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    //Allocate memory: each rank only owns part of the arrays!
    double * const A = new double[n*n/size];
    double * const v = new double[n/size];
    double * const w = new double[n];

    
    //"global" index i goes from 0 to n-1. Each rank owns elements from istart to iend.
    int istart =  rank   *(n/size);
    int iend   = (rank+1)*(n/size);

    // initialize A_ij = (i + 2*j) / n^2
    // initialize v_i  =  1 + 2 / (i+0.5)
    // initialize w_i  =  1 - i / (3*n)
    const double fac0 = 1./(n*n);
    const double fac1 = 1./(3.*n);
    for (int i=istart; i<iend; ++i)
    {
        //Careful with indices! We need i-istart (not just i) for v and A, in order to not get a segmentation fault
        v[i-istart] = 1. + 2. / (i + 0.5);
        for (int j=0; j<n; ++j)
               A[(i-istart)*n + j] = (i + 2.*j) * fac0;
    }
    for (int i=0; i<n; ++i)
    {
        w[i] = 1. - i * fac1;
    }

    //each rank will compute its own part of the sum here
    double myresult = 0.;
    for (int i=istart; i<iend; ++i)
    for (int j=0; j<n; ++j)
        myresult += v[i-istart] * A[(i-istart)*n + j] * w[j];

    //collect the results to rank 0 and print on screen
    double result;
    MPI_Reduce(&myresult,&result,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if (rank == 0)
        std::cout << "Result = " << result << std::endl;

    delete[] A;
    delete[] v;
    delete[] w;

    MPI_Finalize();

    return 0;
}
