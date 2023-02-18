#include <vector>
#include <cmath>
#include <fstream>
#include <cassert>
#include <cstdio>

#include <mpi.h>

// Computes the forces and advances the particles with time step dt
// updating xx,yy with the new positions.
void Step(std::vector<double>& xx, std::vector<double>& yy, double dt) {

    // TODO.a

    // ...
}

// Prints the mean and variance of the radial distance over all particles.
void PrintStat(const std::vector<double>& xx, const std::vector<double>& yy) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double mean = 0.;
    double var = 0.;

    // TODO.b
    // ...

    // Print on root only
    if (rank == 0) {
        printf("mean=%-12.5g var=%.5g\n", mean, var);
    }
}

// Writes lines <x y> to file fn.
void Dump(const std::vector<double>& xx, const std::vector<double>& yy, std::string fn)
{
    int n = xx.size();

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        int na = size * n;
        std::vector<double> xxa(na);
        std::vector<double> yya(na);
        MPI_Gather(xx.data(), n, MPI_DOUBLE, xxa.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(yy.data(), n, MPI_DOUBLE, yya.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        std::ofstream o(fn);
        for (int i = 0; i < na; ++i)
        {
            o << xxa[i] << ' ' << yya[i] << '\n';
        }
    } 
    else 
    {
        MPI_Gather(xx.data(), n, MPI_DOUBLE, nullptr, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Gather(yy.data(), n, MPI_DOUBLE, nullptr, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Parameters.
    const int N = 60;            // total number of particles
    const int NL = N / size;     // particles per rank
    const double dt = 0.1 / N;   // time step

    assert(N % size == 0);

    // Local positions of particles.
    std::vector<double> xx, yy;

    // Seed particles on a unit circle.
    for (int i = rank * NL; i < (rank + 1) * NL; ++i)
    {
        double a = double(i) / N * 2. * M_PI;
        xx.push_back(std::cos(a));
        yy.push_back(std::sin(a));
    }

    PrintStat(xx, yy);
    Dump(xx, yy, "init.dat");

    // Time steps
    for (int t = 0; t < 10; ++t)
    {
        Step(xx, yy, dt);
        PrintStat(xx, yy);
    }

    Dump(xx, yy, "final.dat");

    MPI_Finalize();
}
