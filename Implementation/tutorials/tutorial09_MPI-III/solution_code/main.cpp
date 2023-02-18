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

    int tag = 0;

    int n = xx.size();

    // forces
    std::vector<double> ffx(n, 0.), ffy(n, 0.);
    // buffer for local computation
    std::vector<double> xxt = xx;
    std::vector<double> yyt = yy;
    // buffer for recv
    std::vector<double> xxb(n);
    std::vector<double> yyb(n);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rm = (rank + size - 1) % size;
    int rp = (rank + 1       ) % size;

    for (int k = 0; k < size; ++k)
    {
        const int ne = 4;
        MPI_Request ee[ne];
        if (k != size - 1)
        {
            MPI_Irecv(xxb.data(), n, MPI_DOUBLE, rm, tag, MPI_COMM_WORLD, &ee[0]);
            MPI_Irecv(yyb.data(), n, MPI_DOUBLE, rm, tag, MPI_COMM_WORLD, &ee[1]);
            MPI_Isend(xxt.data(), n, MPI_DOUBLE, rp, tag, MPI_COMM_WORLD, &ee[2]);
            MPI_Isend(yyt.data(), n, MPI_DOUBLE, rp, tag, MPI_COMM_WORLD, &ee[3]);
        }

        for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            const double dx = xx[i] - xxt[j];
            const double dy = yy[i] - yyt[j];
            const double r = std::pow(dx * dx + dy * dy + 1e-20,-1.5);
            ffx[i] += dx * r;
            ffy[i] += dy * r;
        }
        
        if (k != size - 1)
        {
            MPI_Waitall(ne, ee, MPI_STATUSES_IGNORE);
            std::swap(xxt, xxb);
            std::swap(yyt, yyb);
        }
    }

    // advance particles
    for (int i = 0; i < n; ++i)
    {
        xx[i] += dt * ffx[i];
        yy[i] += dt * ffy[i];
    }
}

// Prints the mean and variance of the radial distance over all particles.
void PrintStat(const std::vector<double>& xx, const std::vector<double>& yy) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double mean = 0.;
    double var = 0.;

    // TODO.b

    int n = xx.size();
    for (int i = 0; i < n; ++i)
    {
        const double r = std::sqrt(xx[i] * xx[i] + yy[i] * yy[i]);
        mean += r;
        var += r * r;
    }
    MPI_Reduce(rank == 0 ? MPI_IN_PLACE : &n   , &n   , 1, MPI_INT   , MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(rank == 0 ? MPI_IN_PLACE : &mean, &mean, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(rank == 0 ? MPI_IN_PLACE : &var , &var , 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    mean /= n;
    var /= n;
    var = var - mean * mean;

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
