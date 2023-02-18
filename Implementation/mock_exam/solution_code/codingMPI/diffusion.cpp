#include <fstream>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <vector>

struct Diagnostics
{
    double time;
    double integral;
    Diagnostics(double time, double integral) : time(time), integral(integral){}
};

struct Diffusion1D {
    double D, L; // diffusion constant and domain length
    int N;       // grid points 
    int local_N; // number of grid points of this process

    double h, dt; // grid spacing and timestep
    double aux;   // auxiliary variable

    std::vector<double> u; // solution vector
    std::vector<double> u_tmp;

    int rank, size; // MPI rank and total number of ranks

    std::vector<Diagnostics> diag;

    Diffusion1D(double D, double L, int N, int rank, int size) : D(D), L(L), N(N), rank(rank), size(size)
    {
        h = L / (N - 1);
        dt = h * h / (2.0 * D); // largest possible timestep (larger values lead to instabilities)

        local_N = N / size;
        if (rank == size - 1)
            local_N += N % size; // Correction for the last process, assign remaining points to it

        u.    resize((local_N + 2), 0.0); //+2 for the ghost cells
        u_tmp.resize((local_N + 2), 0.0);

        aux = dt * D / (h * h);

        //First rank does not receive anything from a 'previous' rank. It sets a boundary condition instead
        if (rank==0) u[0]=10.;

        //Last rank does not receive anything from a 'next' rank. It sets a boundary condition instead
        if (rank==size-1) u[local_N+1]=10.;
    }

    void advance()
    {

        // TODO: Implement Blocking MPI communication to exchange the ghost
        // cells on a periodic domain required to compute the central finite
        // differences below.

        // *** start MPI part ***

        int prev_rank = rank - 1;
        int next_rank = rank + 1;

        if (prev_rank < 0    ) prev_rank = MPI_PROC_NULL;
        if (next_rank >= size) next_rank = MPI_PROC_NULL;

        /* Exchange ALL necessary ghost cells with neighboring ranks */
        MPI_Sendrecv(&u[1]      , 1, MPI_DOUBLE, prev_rank, 0, &u[(local_N + 1)], 1, MPI_DOUBLE, next_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(&u[local_N], 1, MPI_DOUBLE, next_rank, 1, &u[0]            , 1, MPI_DOUBLE, prev_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // *** end MPI part ***

        //First rank does not receive anything from a 'previous' rank. It sets a boundary condition instead
        if (rank==0) u[0]=10.;

        //Last rank does not receive anything from a 'next' rank. It sets a boundary condition instead
        if (rank==size-1) u[local_N+1]=10.;

        /* Central differences in space, forward Euler in time, Dirichlet BCs */
        for (int i = 1; i <= local_N; ++i)
            u_tmp[i] = u[i] + aux * (u[i-1] + u[i+1]-2.*u[i]);

        // swap temp with new solution
        std::swap(u_tmp, u);
    }

    void compute_diagnostics(const double t)
    {
        double ammount = 0.0;

        /* Integration to compute total concentration */
        for (int i = 1; i <= local_N; ++i)
            ammount += u[i] * h;

        // TODO: Sum total ammount from all ranks

        // *** start MPI part ***
        MPI_Reduce(rank == 0 ? MPI_IN_PLACE : &ammount, &ammount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        // *** end MPI part ***

        if (rank == 0)
        {
            std::cout << "t = " << t << " ammount = " << ammount << '\n';
            diag.push_back(Diagnostics(t, ammount));
        }
    }

    void write_diagnostics(const std::string& filename) const
    {
        std::ofstream out_file(filename, std::ios::out);
        for (const Diagnostics& d : diag)
            out_file << d.time << ' ' << d.integral << '\n';
        out_file.close();
    }

};

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " D L N \n";
        return 1;
    }

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const double D = std::stod(argv[1]);
    const double L = std::stod(argv[2]);
    const int N = std::stoul(argv[3]);

    if (rank == 0) printf("Running Diffusion 1D on a %d grid with %d ranks.\n", N,  size);

    Diffusion1D system(D, L, N, rank, size);
    system.compute_diagnostics(0);
    for (int step = 0; step < 10000; step++)
    {
        system.advance();
        system.compute_diagnostics(system.dt * step);
    }

    if (rank == 0) system.write_diagnostics("diagnostics.dat");

    MPI_Finalize();

    return 0;
}
