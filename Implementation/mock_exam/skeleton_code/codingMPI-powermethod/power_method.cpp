#include <iostream>
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

int main()
{
    const int N = 8192;

    // matrix initialization
    double* A = new double[N*N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            const double ii = i+1;
            const double jj = j+1;
            A[i*N + j] = lehmer(ii,jj);
        }
    }

    double* q0 = new double[N];
    double* q1 = new double[N];

    // initial guess
    q0[0] = 1.0;
    for (int i = 1; i < N; i++)
        q0[i] = 0.0;

    const double tol = 1.0e-6;
    double lambda0 = 1.0e12;
    double lambda1 = 0.0;
    size_t iter = 0;
    _gemv(N, N, A, q0, q1); 
    while (true)
    {
        _norm(N, q1);
        _gemv(N, N, A, q1, q0);
        lambda1 = 0.0;
        for (int i = 0; i < N; ++i)
            lambda1 += q0[i] * q1[i];
        ++iter;
        if (std::abs(lambda1 - lambda0) < tol)
            break;
        lambda0 = lambda1;
        std::swap(q0, q1);
    }
    std::cout << "Largest eigenvalue = " << lambda1 << " (iterations = " << iter << ")" << std::endl;

    delete [] A;
    delete [] q0;
    delete [] q1;

	return 0;
}
