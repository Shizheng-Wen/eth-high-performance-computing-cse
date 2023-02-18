#include <iostream>
#include <iomanip>
#include <functional>
#include <random>
#include <cstdint>
#include <cmath>
#include <omp.h>
#include <cblas.h>
#include <sys/time.h>

double benchmark(std::function<void()>& kernel)
{
    kernel();// warm-up

    double time = -omp_get_wtime();
    kernel();
    time += omp_get_wtime();

    return time;
}

double norm(const int N, const double * const M)
{
    double sum = 0.0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            sum += M[i*N+j]*M[i*N+j];

    return std::sqrt(sum);
}

void dgemm(const int N, double * const __restrict C, const double * const __restrict A, const double * const __restrict B)
{
    for (int i = 0; i < N; ++i)
    {
        double Aik = A[i*N];
        for (int j = 0; j < N; ++j)
            C[i*N + j] = Aik * B[j];
        for (int k = 1; k < N; ++k)
        {
            Aik = A[i*N + k];
            for (int j = 0; j < N; ++j)
                C[i*N + j] += Aik * B[k*N + j];
        }
    }
}

void set_random(const int N, double * const A, double * const B, double * const C)
{
        std::default_random_engine gen(0);
        std::uniform_real_distribution<double> rng(1,10);

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                A[i*N + j] = rng(gen);

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                B[i*N + j] = rng(gen);

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                C[i*N + j] = 0.0;
}


int main(int argc, char* argv[])
{
    for (int i=10, k=0; i<12; ++i, ++k)
    {
        const unsigned int N = 1<<i;
        double * const A = new double[N*N];
        double * const B = new double[N*N];
        double * const C = new double[N*N];

        std::cout << "Running " << N << " x " << N << std::endl;

        //openblas_set_num_threads(24);
        //std::cout << "cblas running with " <<  openblas_get_num_threads() << " threads."<< std::endl;
        //std::cout << openblas_get_config() << std::endl;

        std::function<void()> fblas = [=]()
        {
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, A, N, B, N, 0.0, C, N);
        };

        std::function<void()> fnaive = [=]()
        {
            dgemm(N, C, A, B);
        };

        set_random(N, A, B, C);
        const double gold = benchmark(fblas);
        const double gnorm = norm(N, C);
        std::cout << "blas = " << std::fixed << gold << std::endl;

        set_random(N, A, B, C);
        const double test = benchmark(fnaive);
        const double tnorm = norm(N, C);
        std::cout << "test = " << std::fixed << test << std::endl;

        std::cout << "Difference L2 norm = " << std::scientific << tnorm - gnorm << std::endl;
        std::cout << "Ratio naive / blas = " << std::scientific << test/gold << std::endl;

        delete[] A;
        delete[] B;
        delete[] C;
    }

    return 0;
}
