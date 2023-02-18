#include <iostream>
#include <cassert>
#include <cmath>
#include <mpi.h>

#define lehmer(i,j) ((i) <= (j))? ((i)/(j)):((j)/(i))

inline void _gemv(const int m, const int n, const double* const A, const double* const x, double* const y)
{
    for (int i = 0; i < m; ++i)
    {
        double sum = 0.0;
        for (int j = 0; j < n; ++j)
            sum += A[i*n + j] * x[j];
        y[i] = sum;
    }
}

inline void _norm(const int n, double* const y)
{
    double sum2 = 0.0;
    for (int i = 0; i < n; ++i)
        sum2 += y[i] * y[i];
    sum2 = 1.0 / std::sqrt(sum2);
    for (int i = 0; i < n; ++i)
        y[i] *= sum2;
}

int main(int argc, char* argv[])
{
    const int N = 8192;

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    assert(N%size == 0); // keep it simple
    const int myN = N/size;
    const int off = rank * myN;

    // matrix initialization (rank tiled)
    double* A = new double[myN*N];

    for (int i = 0; i < myN; i++) {
        for (int j = 0; j < N; j++) {
            const double ii = i+off+1;
            const double jj = j+1;
            A[i*N + j] = lehmer(ii,jj);
        }
    }

    double* q0 = new double[N]; // auxiliary storage
    double* q1 = new double[N]; // auxiliary storage

    // initial guess (all ranks start with this)
    q0[0] = 1.0;
    for (int i = 1; i < N; i++)
        q0[i] = 0.0;

    const double tol = 1.0e-6;
    double lambda0 = 1.0e12;
    double lambda1 = 0.0;
    size_t iter = 0;
    _gemv(myN, N, A, q0, q1);
    while (true)
    {
        MPI_Allgather(q1, myN, MPI_DOUBLE, q0, myN, MPI_DOUBLE, MPI_COMM_WORLD);
        _norm(N, q0);
        _gemv(myN, N, A, q0, q1);
        lambda1 = 0.0;
        for (int i = 0; i < myN; ++i)
            lambda1 += q0[off+i] * q1[i];
        MPI_Allreduce(MPI_IN_PLACE, &lambda1, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        ++iter;
        if (std::abs(lambda1 - lambda0) < tol)
            break;
        lambda0 = lambda1;
    }

    if (0 == rank)
        std::cout << "Largest eigenvalue = " << lambda1 << " (iterations = " << iter << ")" << std::endl;

    delete [] A;
    delete [] q0;
    delete [] q1;

    MPI_Finalize();

    return 0;
}
