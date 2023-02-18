#include <iostream>
#include <random>
#include <mpi.h>

double F(double x, double y) // Integrand
{
      if (x * x + y * y < 1.) return 4.; // inside unit circle 
      return 0.;
}

double C0(int n, int seed = 0)
{
    double sum = 0.;

    std::default_random_engine g(seed);  // random generator with seed 0

    for (int i = 0; i < n; ++i)
    {
        std::uniform_real_distribution<double> u; // uniform distribution in [0, 1]
        double x = u(g);
        double y = u(g);
        sum += F(x, y);
    }
    return sum/n;
}

int main(int argc, char *argv[])
{
    MPI_Init (&argc,&argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int n = (argc > 1 ? atoi(argv[1]) : 1e8); // number of samples (default value is 10^8)

    if (rank == 0) std::cout << "Running with " << size << " ranks and " << n << " samples.\n";

    double ref = 3.14159265358979323846; // reference solution

    double t0 = MPI_Wtime();

    //each rank will perform n/size of the total of n samples
    //also, provide different seed for each rank
    double myres = C0(n/size,rank) / size; //need to divide result by "size"!

    double t1 = MPI_Wtime();

    double res;

    MPI_Reduce(&myres,&res,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD); //collect partial results from each rank to rank 0.

    if (rank == 0) std::cout << "res = " << res << " error = " << res-ref << " time = " << t1 - t0 << "\n";

    MPI_Finalize();
    return 0;
}
