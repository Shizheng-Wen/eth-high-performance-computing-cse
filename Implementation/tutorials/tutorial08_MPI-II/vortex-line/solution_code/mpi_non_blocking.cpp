// Copyright 2022 ETH Zurich. All Rights Reserved.

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

#include <mpi.h>

static double gammaInit(double x)
{
    return 4 * x / std::sqrt(1 - 4 * x * x);
}

static void initialConditions(double start, double end, std::vector<double>& x,
                              std::vector<double>& y,
                              std::vector<double>& gamma)
{
    const int n = (int)gamma.size();
    const double dx = (end - start) / n;

    for (int i = 0; i < n; ++i) {
        x[i] = start + dx * (i + 0.5);
        y[i] = 0;
        gamma[i] = dx * gammaInit(x[i]);
    }
}

static void addVelocities(double epsSq, const std::vector<double>& xLocal,
                          const std::vector<double>& yLocal,
                          const std::vector<double>& x,
                          const std::vector<double>& y,
                          const std::vector<double>& gamma,
                          std::vector<double>& u, std::vector<double>& v)
{
    const int nLocal = (int)xLocal.size();
    const int nRemote = (int)x.size();

    constexpr double inv2pi = 1.0 / (2 * M_PI);

    for (int i = 0; i < nLocal; ++i) {
        for (int j = 0; j < nRemote; ++j) {
            const double dx = xLocal[i] - x[j];
            const double dy = yLocal[i] - y[j];
            const double g = gamma[j];

            const double factor = inv2pi * g / (epsSq + dx * dx + dy * dy);

            u[i] -= dy * factor;
            v[i] += dx * factor;
        }
    }
}

static void computeVelocities(MPI_Comm comm, double epsSq,
                              const std::vector<double>& x,
                              const std::vector<double>& y,
                              const std::vector<double>& gamma,
                              std::vector<double>& u, std::vector<double>& v)
{
    const int n = (int)x.size();
    u.assign(n, 0.0);
    v.assign(n, 0.0);

    std::vector<double> xremote(n), yremote(n), gammaremote(n);
    std::vector<double> xrecv(n), yrecv(n), gammarecv(n);

    // the first pass is the self interaction.
    xremote = x;
    yremote = y;
    gammaremote = gamma;

    int rank, nranks;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nranks);

    MPI_Request sreqs[3], rreqs[3];

    // note: starts at 1, not 0
    for (int pass = 1; pass < nranks; ++pass) {
        const int dst = (rank + pass + nranks) % nranks;
        const int src = (rank - pass + nranks) % nranks;
        const int tag = 42;

        MPI_Irecv(xrecv.data(), n, MPI_DOUBLE, src, tag, comm, rreqs + 0);
        MPI_Irecv(yrecv.data(), n, MPI_DOUBLE, src, tag, comm, rreqs + 1);
        MPI_Irecv(gammarecv.data(), n, MPI_DOUBLE, src, tag, comm, rreqs + 2);

        MPI_Isend(x.data(), n, MPI_DOUBLE, dst, tag, comm, sreqs + 0);
        MPI_Isend(y.data(), n, MPI_DOUBLE, dst, tag, comm, sreqs + 1);
        MPI_Isend(gamma.data(), n, MPI_DOUBLE, dst, tag, comm, sreqs + 2);

        // overlap communication with computation
        addVelocities(epsSq, x, y, xremote, yremote, gammaremote, u, v);

        MPI_Waitall(3, rreqs, MPI_STATUSES_IGNORE);
        MPI_Waitall(3, sreqs, MPI_STATUSES_IGNORE);

        std::swap(xrecv, xremote);
        std::swap(yrecv, yremote);
        std::swap(gammarecv, gammaremote);
    }
    // last pass with the latest recved data
    addVelocities(epsSq, x, y, xremote, yremote, gammaremote, u, v);
}

static void forwardEuler(double dt, const std::vector<double>& u,
                         const std::vector<double>& v, std::vector<double>& x,
                         std::vector<double>& y)
{
    for (int i = 0; i < (int)x.size(); ++i) {
        x[i] += dt * u[i];
        y[i] += dt * v[i];
    }
}

static void dumpToCsv(int step, const std::vector<double>& x,
                      const std::vector<double>& y,
                      const std::vector<double>& gamma)
{
    char fname[128];
    sprintf(fname, "config_%05d.csv", step);

    FILE* f = fopen(fname, "wb");
    fprintf(f, "x,y,gamma\n");
    for (int i = 0; i < (int)x.size(); ++i) {
        fprintf(f, "%g,%g,%g\n", x[i], y[i], gamma[i]);
    }
    fclose(f);
}

static void dumpToCsv(MPI_Comm comm, int step, const std::vector<double>& x,
                      const std::vector<double>& y,
                      const std::vector<double>& gamma)
{
    int rank, nranks;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nranks);

    std::vector<double> xAll, yAll, gammaAll;

    if (rank == 0) {
        const auto n = x.size() * nranks;
        xAll.resize(n);
        yAll.resize(n);
        gammaAll.resize(n);
    }

    const int n = x.size();
    MPI_Gather(x.data(), n, MPI_DOUBLE, xAll.data(), n, MPI_DOUBLE, 0, comm);
    MPI_Gather(y.data(), n, MPI_DOUBLE, yAll.data(), n, MPI_DOUBLE, 0, comm);
    MPI_Gather(gamma.data(), n, MPI_DOUBLE, gammaAll.data(), n, MPI_DOUBLE, 0,
               comm);

    if (rank == 0)
        dumpToCsv(step, xAll, yAll, gammaAll);
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    if (argc != 2) {
        fprintf(stderr, "usage: %s <total number of particles>\n", argv[0]);
        exit(1);
    }

    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, nranks;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nranks);

    const int nglobal = std::atoi(argv[1]);

    if (nglobal % nranks != 0) {
        fprintf(stderr,
                "expected n to be a multiple of the number of ranks.\n");
        exit(1);
    }

    const int n = nglobal / nranks;
    const double extents = 1.0 / nranks;

    const double startX = -0.5 + rank * extents;
    const double endX = startX + extents;

    const double dt = 1e-4;
    const double epsSq = 1e-3;
    const double endTime = 2.5;
    const double dumpFreq = 0.1;

    const int dumpEvery = dumpFreq / dt;
    const int numSteps = endTime / dt;

    // state of the simulation
    std::vector<double> x(n);
    std::vector<double> y(n);
    std::vector<double> gamma(n);

    // workspace
    std::vector<double> u(n);
    std::vector<double> v(n);

    initialConditions(startX, endX, x, y, gamma);

    for (int step = 0; step < numSteps; ++step) {
        if (step % dumpEvery == 0) {
            const int id = step / dumpEvery;
            dumpToCsv(comm, id, x, y, gamma);
        }

        computeVelocities(comm, epsSq, x, y, gamma, u, v);
        forwardEuler(dt, u, v, x, y);
    }

    MPI_Finalize();

    return 0;
}
