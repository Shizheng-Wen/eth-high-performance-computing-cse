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
    double amount;    // total mass
    double min_c, max_c; // min,max concentration values

    std::vector<double> c;     // solution vector
    std::vector<double> c_tmp; // temporary solution vector
    std::vector<Diagnostics> diag; // vector to store concentration values


    Diffusion(double D, double L, size_t N) : D(D), L(L), N(N) {
        h = L / (N - 1);
        dt = h * h / (2.0 * D);

        c.resize(N, 0.0);
        c_tmp.resize(N, 0.0);

        aux = dt * D / (h * h);
        initialize_density();
    }


    void advance() {
        /* Central differences in space, forward Euler in time, Dirichlet BCs */
        #pragma omp for
        for (size_t j=1; j<N-1; ++j)
            c_tmp[j] = c[j] + aux * ( c[j+1] - 2*c[j] + c[j-1] );

        #pragma omp single
        std::swap(c_tmp, c);
    }


    void compute_diagnostics(const double t) {

        /* Integration to compute total concentration */
        #pragma omp single
        amount = 0.;

        #pragma omp for reduction(+ : amount)
        for (size_t j=1; j<N-1; ++j)
            amount += c[j];

        #pragma omp single nowait
        {
        amount *= h;  // Concentration = Mass / Volume
        printf("t = %lf amount = %lf\n", t, amount);
        }

    }


    void compute_histogram(std::vector<int> &hist) {
        /* number of bins */
        const size_t M = hist.size();

        /* Initialize max and min concentration */
        #pragma omp single
        {
        max_c = c[1];
        min_c = c[1];
        }

        /* Find max and min concentration values */
        #pragma omp for reduction(min : min_c) reduction(max : max_c)
        for (size_t j=1; j<N-1; ++j) {
            double c0 = c[j];
            max_c = std::max(max_c, c0);
            min_c = std::min(min_c, c0);
        }

        const double epsilon = 1e-8;
        double dc = (max_c - min_c + epsilon) / M;

        /* Accumulate equispaced bins */
        // local calculation of bins
        std::vector<int> local_hist(M, 0);
        #pragma omp for nowait
        for (size_t j=1; j<N-1; ++j) {
            size_t bin = (c[j] - min_c) / dc;
            local_hist[bin]++;
        }


        // global aggregation of local histograms
        #pragma omp critical
        {
        for (size_t i = 0; i < M; ++i)
            hist[i] += local_hist[i];
        }
    }


    void initialize_density() {
        const double bound = 0.25 * L;
        
        #pragma omp parallel for
        for (size_t j=0; j<N; ++j) {
          if (std::abs(j * h - 0.5 * L) < bound)
            c[j+1] = 1;
          else
            c[j+1] = 0;
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
    if (argc < 2) {
        fprintf(stderr, "Usage: %s N \n", argv[0]);
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

    int numThreads = omp_get_num_threads();
    printf("Running Diffusion 2D on a %zu x %zu grid with %d threads.\n", N, N, numThreads);

    /* Initialize diffusion system */
    Diffusion system(D, L, N);

    /* System evolution in time */
    size_t numSteps = (T / system.dt + 1);
    std::vector<int> hist(10, 0); // histogram with 10 bins
    auto tstart = std::chrono::steady_clock::now();

    system.min_c = system.c[1];
    system.max_c = system.c[1];
    #pragma omp parallel
    {
    for (size_t step = 0; step <= numSteps; ++step) {
        double t = system.dt * step;

        system.compute_diagnostics(t);

        #pragma omp single 
        std::fill(hist.begin(), hist.end(), 0);

        system.compute_histogram(hist);

        #pragma omp single nowait // or omp master
        system.diag.push_back(Diagnostics(t, system.amount, hist));

        system.advance();
        }
        #pragma omp barrier // not necessary (implicit barrier in single)
    }
    auto tend = std::chrono::steady_clock::now();
    double ms = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart).count();

    /* Write diagnostics */
    // Uncomment to write diagnostics: system.write_diagnostics("statistics.dat");
    printf("time: %lf\n", ms);

    return 0;
}
