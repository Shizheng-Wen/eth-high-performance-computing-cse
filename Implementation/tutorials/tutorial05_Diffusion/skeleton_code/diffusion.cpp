#include <chrono>
#include <cmath>
#include <fstream>
#include <omp.h>
#include <vector>



struct Diagnostics {
    double time;
    double concentration;
    std::vector<int> histogram;
    
    Diagnostics(double time, double concentration, std::vector<int> &histogram)
        : time(time), concentration(concentration), histogram(histogram) {}
};



struct Diffusion {
    const double D;   // diffusion constant
    const double L;   // domain length
    size_t N;         // grid points per direction (whole grid is NxN)
    double h, dt;     // grid spacing and timestep
    double aux;       // auxiliary variable (prefactor)

    std::vector<double> c;     // solution vector
    std::vector<double> c_tmp; // temporary solution vector
    std::vector<Diagnostics> diag; // vector to store concentration values


    Diffusion(double D, double L, size_t N) : D(D), L(L), N(N) {
        h  = // TODO: Find h
        dt = // TODO: Find max value of dt

        // TODO: Resize the vectors c and c_tmp, and set all elements to zero
        // Note: Do not forget the ghost nodes!!
        c.resize( ?? );
        c_tmp.resize( ?? );

        aux = // TODO: Compute the prefactor in the discretized diffusion eq.

        initialize_density();
    }


    void advance() {
        // TODO: (1) Implement time advancement
        // - Loop over grid points
        // - Compute the concentration at a grid point j, at the next time step:
        //   > Implement central difference in space, forward Euler in time
        //   > adjust the boundary values based on the boundary conditions
        // - Update old and new solution vectors

        // TODO: (2) Parallelize diffusion with OpenMP
    }


    double compute_diagnostics(const double t) {
        double amount = 0.0;

        // TODO: Compute total concentration (integration)
        // TODO: Parallelize integration with OpenMP
        for ...
            amount += ...



        amount *= h;  // Concentration = Mass / Volume

        printf("t = %lf amount = %lf\n", t, amount);
        return amount;
    }


    void compute_histogram(std::vector<int> &hist) {
        /* number of bins */
        const size_t M = hist.size();

        /* Initialize max and min concentration */
        double max_c, min_c;
        max_c = c[1];
        min_c = c[1];

        // TODO: Find max and min concentration values
        //  > Note: what are the bounds of this for loop?
        // TODO: Parallelize max_c and min_c initialization
        for ...


        const double epsilon = 1e-8;
        double dc = (max_c - min_c + epsilon) / M;


        // TODO: Compute concentration histogram
        //  - Loop over grid
        //  - Histogram: Compute number of grid points having a particular concentration
        // TODO: Parallelize bin accumulation

        for (size_t j = 1; j <= N; ++j) {
            size_t bin = ...
            hist[bin]  = ...
        }
    }


    void initialize_density() {

        // TODO: Loop over grid and set initial concentration values
        // based on the Inictial Condition
        const double bound = 0.25 * L;

        for  ...  {
            ...
            c[j] = ...
        }
    }


    void write_diagnostics(const std::string &filename) const {
        // write header
        std::ofstream out_file(filename, std::ios::out);
        out_file << "t concentration";
        for (size_t i = 0; i < diag[0].histogram.size(); ++i)
            out_file << " bin" << i;
        out_file << '\n';

        // write data
        for (const Diagnostics &d : diag) {
            out_file << d.time << ' ' << d.concentration;
            for (auto h : d.histogram)
                out_file << ' ' << h;
            out_file << '\n';
        }
        out_file.close();
  }

}; // end struct Diffusion



int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s N out\n", argv[0]);
        return 1;
    }

    /* Set up diffusion parameters */
    //
    /* Init diffusion constant */
    const double D = 1;

    /* Init domain size */
    const double L = 1;

    /* Init simulation length */
    const double T = 1;

    /* Init num grid points */
    const size_t N = std::stoul(argv[1]);

    /* Produce output */
    const size_t out = std::stoul(argv[2]); // YES == 1

    int numThreads = omp_get_num_threads();
    printf("Running Diffusion 2D on a %zu x %zu grid with %d threads.\n", N, N,
           numThreads);


    /* Initialize diffusion system */
    Diffusion system(D, L, N);


    /* System evolution in time */
    size_t numSteps = (T / system.dt + 1);
    auto tstart = std::chrono::steady_clock::now();
    for (size_t step = 0; step <= numSteps; ++step) {
        double t = system.dt * step;
        double amount = system.compute_diagnostics(t);

        std::vector<int> hist(10, 0); // histogram with 10 bins
        system.compute_histogram(hist);

        system.diag.push_back(Diagnostics(t, amount, hist));

        system.advance();
    }
    auto tend = std::chrono::steady_clock::now();
    double ms = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart).count();


    /* Write diagnostics */
    if (out == 1)
        system.write_diagnostics("diagnostics.dat");

    printf("time: %lf\n", ms);

    return 0;
}
