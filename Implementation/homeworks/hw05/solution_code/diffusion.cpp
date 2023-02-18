#include <fstream>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <vector>

struct Diagnostics {
    double time;
    double concentration;

    Diagnostics(double time, double concentration)
        : time(time), concentration(concentration)
    {
    }
};

struct Diffusion {
    double D, L; // diffusion constant and domain length
    int N;       // grid points per direction (whole grid is NxN)
    int local_N; // number of rows of this process

    double h, dt; // grid spacing and timestep
    double aux;   // auxiliary variable

    std::vector<double> c; // solution vector
    std::vector<double> c_tmp;

    int rank, size; // MPI rank and total number of ranks

    std::vector<Diagnostics> diag;

    Diffusion(double D, double L, int N, int rank, int size)
        : D(D), L(L), N(N), rank(rank), size(size)
    {
        h = L / (N - 1);
        dt = h * h / (4.0 * D); // this is the largest possible timestep (larger
                                // values lead to instabilities)

        local_N = N / size;
        if (rank == size - 1)
            local_N += N % size; // Correction for the last process

        c.resize((local_N + 2) * (N + 2), 0.0); //+2 for the ghost cells
        c_tmp.resize((local_N + 2) * (N + 2), 0.0);

        aux = dt * D / (h * h);
        initialize();
    }

    void advance()
    {

        // TODO: Implement Blocking MPI communication to exchange the ghost
        // cells required to compute the central finite
        // differences below. Watch out for the Dirchlet boundary condition!
        // Either use MPI_Sendrecv or MPI_Send & MPI_Recv
        // MPI_PROC_NULL can be used to communicate to non existing ranks (like at the bounderies)

        // *** start MPI part ***
        MPI_Status status[2];

        int prev_rank = rank - 1;
        int next_rank = rank + 1;

        if (prev_rank < 0)
            prev_rank = MPI_PROC_NULL;
        if (next_rank >= size)
            next_rank = MPI_PROC_NULL;

        /* Exchange ALL necessary ghost cells with neighboring ranks */
        MPI_Sendrecv(&c[1 * (N + 2) + 1], N, MPI_DOUBLE, prev_rank, 0,
                     &c[(local_N + 1) * (N + 2) + 1], N, MPI_DOUBLE, next_rank,
                     0, MPI_COMM_WORLD, &status[0]);

        MPI_Sendrecv(&c[local_N * (N + 2) + 1], N, MPI_DOUBLE, next_rank, 1,
                     &c[0 * (N + 2) + 1], N, MPI_DOUBLE, prev_rank, 1,
                     MPI_COMM_WORLD, &status[1]);

        
        //other solution with just MPI_Isend & MPI_Irecv
        
        if(size > 1){
            std::vector<MPI_Request> requests(4*(size-1), MPI_REQUEST_NULL);
            int tag = 42;
            if(rank == 0){
                MPI_Isend(&c[local_N*(N + 2) + 1], N, MPI_DOUBLE, rank+1, tag, MPI_COMM_WORLD, &requests[0]);
                
                MPI_Irecv(&c[(local_N+1)*(N + 2) + 1], N, MPI_DOUBLE, rank+1, tag, MPI_COMM_WORLD, &requests[1]);
            }else if(rank == size - 1){
                MPI_Isend(&c[(N + 2) + 1], N, MPI_DOUBLE, rank-1, tag, MPI_COMM_WORLD, &requests[2]);
                
                MPI_Irecv(&c[1], N, MPI_DOUBLE, rank-1, tag, MPI_COMM_WORLD, &requests[3]);
            }else{
                MPI_Isend(&c[(N + 2) + 1], N, MPI_DOUBLE, rank-1, tag, MPI_COMM_WORLD, &requests[4*rank]);
                MPI_Isend(&c[local_N*(N + 2) + 1], N, MPI_DOUBLE, rank+1, tag, MPI_COMM_WORLD, &requests[4*rank+1]);
                
                MPI_Irecv(&c[(local_N+1)*(N + 2) + 1], N, MPI_DOUBLE, rank+1, tag, MPI_COMM_WORLD, &requests[4*rank+2]);
                MPI_Irecv(&c[1], N, MPI_DOUBLE, rank-1, tag, MPI_COMM_WORLD, &requests[4*rank+3]);
            }
            MPI_Waitall(requests.size(), requests.data(), MPI_STATUS_IGNORE);
        }
        
        // *** end MPI part ***

        /* Central differences in space, forward Euler in time, Dirichlet BCs */
        for (int i = 1; i <= local_N; ++i)
            for (int j = 1; j <= N; ++j)
                c_tmp[i * (N + 2) + j] =
                    c[i * (N + 2) + j] +
                    aux * (c[i * (N + 2) + (j + 1)] + c[i * (N + 2) + (j - 1)] +
                           c[(i + 1) * (N + 2) + j] + c[(i - 1) * (N + 2) + j] -
                           4 * c[i * (N + 2) + j]);
        using std::swap;
        swap(c_tmp, c);
    }

    void compute_diagnostics(const double t)
    {
        double ammount = 0.0;

        /* Integration to compute total concentration */
        for (int i = 1; i <= local_N; ++i)
            for (int j = 1; j <= N; ++j)
                ammount += c[i * (N + 2) + j] * h * h;

        // TODO: Sum total ammount from all ranks

        // *** start MPI part ***
        MPI_Reduce(rank == 0 ? MPI_IN_PLACE : &ammount, &ammount, 1, MPI_DOUBLE,
                   MPI_SUM, 0, MPI_COMM_WORLD);
        // *** end MPI part ***

        if (rank == 0) {
            std::cout << "t = " << t << " ammount = " << ammount << '\n';
            diag.push_back(Diagnostics(t, ammount));
        }
    }

    void write_diagnostics(const std::string& filename) const
    {
        std::ofstream out_file(filename, std::ios::out);
        for (const Diagnostics& d : diag)
            out_file << d.time << ' ' << d.concentration << '\n';
        out_file.close();
    }

    void compute_histogram()
    {
        /* Number of histogram bins */
        const int M = 10;
        std::vector<int> hist(M, 0);

        /* Find max and min concentration values */
        double max_c, min_c, c0;
        max_c = c[1 * (N + 2) + 1];
        min_c = c[1 * (N + 2) + 1];

        for (int i = 1; i <= local_N; ++i)
            for (int j = 1; j <= N; ++j) {
                c0 = c[i * (N + 2) + j];
                if (c0 > max_c)
                    max_c = c0;
                if (c0 < min_c)
                    min_c = c0;
            }

        // TODO: Compute the global min and max concentration values on this
        // myRank and store the result in min_c and max_c
        double lmin_c = min_c;
        double lmax_c = max_c;

        // *** start MPI part ***
        MPI_Allreduce(&lmin_c, &min_c, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
        MPI_Allreduce(&lmax_c, &max_c, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        // *** end MPI part ***

        double epsilon = 1e-8;
        double dh = (max_c - min_c + epsilon) / M;

        for (int i = 1; i <= local_N; ++i){
            for (int j = 1; j <= N; ++j) {
                int bin = (c[i * (N + 2) + j] - min_c) / dh;
                hist[bin]++;
            }
        }

        // TODO: Compute the sum of the histogram bins over all ranks and store
        // the result in the array g_hist.  Only myRank 0 must print the result.
        std::vector<int> g_hist(M, 0);
        // *** start MPI part ***
        MPI_Reduce(hist.data(), g_hist.data(), M, MPI_INT, MPI_SUM, 0,
                   MPI_COMM_WORLD);
        // *** end MPI part ***

        if (rank == 0) {
            printf("=====================================\n");
            printf("Output of compute_histogram():\n");
            int gl = 0;
            for (int i = 0; i < M; i++) {
                printf("bin[%d] = %d\n", i, g_hist[i]);
                gl += g_hist[i];
            }
            printf("Total elements = %d\n", gl);
        }

    } // end public

    void initialize()
    {
        int gi; // global index
        double bound = 0.25 * L;

        for (int i = 0; i < local_N; ++i) {
            gi = rank * (N / size) + i; // convert local index to global index

            for (int j = 0; j < N; ++j) {
                if (fabs(gi * h - 0.5 * L) < bound &&
                    fabs(j * h - 0.5 * L) < bound)
                    c[(i + 1) * (N + 2) + (j + 1)] = 1;
                else
                    c[(i + 1) * (N + 2) + (j + 1)] = 0;
            }
        }
    }

    int dump(int step)
    {
        int gi, i, j;
        double x, y, c0;
        char path[999];
        FILE* file;
        sprintf(path, "%07d.%03d", step, rank);
        if ((file = fopen(path, "w")) == NULL) {
            fprintf(stderr, "failed to write to '%s'\n", path);
            return 1;
        }
        for (i = 0; i < local_N; ++i) {
            gi = rank * (N / size) + i;
            for (j = 0; j < N; ++j) {
                x = gi * h;
                y = j * h;
                c0 = c[(i + 1) * (N + 2) + (j + 1)];
                if (fprintf(file, "%.16e %.16e %.16e\n", x, y, c0) < 0) {
                    fprintf(stderr, "failed to write to '%s'\n", path);
                    return 1;
                }
            }
        }
        return 0;
    }
};

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " D L N \n";
        return 1;
    }

    // TODO: Start-up the MPI environment and determine this process' myRank ID
    // as well as the total number of processes (=ranks) involved in the
    // communicator

    int rank, size;
    // *** start MPI part ***
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // *** end MPI part ***

    const double D = std::stod(argv[1]);
    const double L = std::stod(argv[2]);
    const int N = std::stoul(argv[3]);

    if (rank == 0)
        printf("Running Diffusion 2D on a %d x %d grid with %d ranks.\n", N, N,
               size);

    Diffusion system(D, L, N, rank, size);

    system.compute_diagnostics(0);

    double t0 = MPI_Wtime();
    for (int step = 0;; step++) {
        if (step % 1000 == 0){
            //system.dump(step);
        }
        if (system.dt * step >= 0.5 || step > 1000)
            break;
        system.advance();
        //system.compute_diagnostics(system.dt * step);
    }
    double t1 = MPI_Wtime();

    system.compute_histogram();

    if (rank == 0){
        system.write_diagnostics("diagnostics.dat");

        std::string file_name = "out_";
        file_name += std::to_string(N);
        file_name += "/";
        file_name += std::to_string(size);
        file_name += ".txt";
        //output time in a file
        std::ofstream myfile;
        myfile.open (file_name);
        myfile << size << " " << t1 - t0 << "\n";
        myfile.close();

        std::cout << "dt: " << system.dt << "\n";
    }
    // TODO: Shutdown the MPI environment
    // *** start MPI part ***
    MPI_Finalize();
    // *** end MPI part ***
    return 0;
}
