#include <iostream>
#include <mpi.h>

//correct result is 39159.4

int main(int argc, char** argv)
{
    const int n = 32768;

    double * const A = new double[n*n];
    double * const v = new double[n];
    double * const w = new double[n];

    // initialize A_ij = (i + 2*j) / n^2
    // initialize v_i  =  1 + 2 / (i+0.5)
    // initialize w_i  =  1 - i / (3*n)
    const double fac0 = 1./(n*n);
    const double fac1 = 1./(3.*n);
    for (int i=0; i<n; ++i)
    {
         v[i] = 1. + 2. / (i + 0.5);
         w[i] = 1. - i * fac1;
         for (int j=0; j<n; ++j)
              A[i*n + j] = (i + 2.*j) * fac0;
    }

    double myresult = 0.;
    for (int i=0; i<n; ++i)
    for (int j=0; j<n; ++j)
         myresult += v[i] * A[i*n + j] * w[j];

    std::cout << "Result = " << myresult << std::endl;

    delete[] A;
    delete[] v;
    delete[] w;

    return 0;
}
