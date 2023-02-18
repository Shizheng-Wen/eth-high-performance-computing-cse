#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <cmath>
#include "timer.hpp"

// Include OpenMP header
#include <omp.h>


struct Diagnostics
{
    double time;
    double heat;

    Diagnostics(double time, double heat) : time(time), heat(heat) {}
};

class Diffusion2D
{
public:
    Diffusion2D(const double D,
                const double L,
                const int N,
                const double dt,
                const int rank)
            : D_(D), L_(L), N_(N), dt_(dt), rank_(rank)
    {
        // Real space grid spacing.
        dr_ = L_ / (N_ - 1);

        // Actual dimension of a row (+2 for the ghost cells).
        real_N_ = N + 2;

        // Total number of cells.
        Ntot_ = (N_ + 2) * (N_ + 2);

        rho_.resize(Ntot_, 0.);
        rhs_.resize(Ntot_, 0.);

        // Initialize field on grid
        initialize_rho();

        // Common prefactor
        R_ = D * dt / (2. * dr_ * dr_);

        // Initialize diagonals of the coefficient
        // matrix A, where Ax=b is the corresponding
        // system to be solved
        a_.resize(real_N_-2, -R_);
        b_.resize(real_N_-2, 1. + 2.*R_);
        c_.resize(real_N_-2, -R_);
    }



    void advance()
    {

        /*
         TODO: Subquestion 3(c):
               Paralelize the computations in this
               function with OpenMP
        */


        // ADI Step 1:
        // The following loops update the elements of rhs_
        // based on values of rho_
        #pragma omp parallel for
        for (int iy=1; iy<real_N_-1; iy++)
        for (int ix=1; ix<real_N_-1; ix++)
        {
            int k  =  iy    * real_N_ + ix;
            int k1 = (iy-1) * real_N_ + ix;
            int k2 = (iy+1) * real_N_ + ix;
            rhs_[k] = rho_[k] + R_ * (rho_[k1] - 2.*rho_[k] + rho_[k2]);
        }

        // The following function is thread-safe and 
        // updates the values of rho_
        #pragma omp parallel for
        for (int iy=1; iy<real_N_-1; iy++)
            thomas(0, iy);



        // ADI Step 2:
        // The following loops update the elements of rhs_
        // based on values of rho_
        #pragma omp parallel for
        for (int iy=1; iy<real_N_-1; iy++)
        for (int ix=1; ix<real_N_-1; ix++)
        {
            int k  = iy * real_N_ + ix;
            int k1 = iy * real_N_ + (ix-1);
            int k2 = iy * real_N_ + (ix+1);
            rhs_[k] = rho_[k] + R_ * (rho_[k1] - 2.*rho_[k] + rho_[k2]);
        }

        // The following function is thread-safe and 
        // updates the values of rho_
        #pragma omp parallel for
        for (int ix=1; ix<real_N_-1; ix++)
            thomas(1, ix);
    }



    void compute_diagnostics(const double t, const int step)
    {

        /*
         TODO: Subquestion 3(d):
               Paralelize the computation of "heat" with OpenMP
        */

        double heat = 0.0;
        #pragma omp parallel for reduction(+:heat)
        for (int i = 1; i < real_N_-1; ++i)
        for (int j = 1; j < real_N_-1; ++j)
            heat += dr_ * dr_ * rho_[i * real_N_ + j];

        std::cout << "t = " << t << " heat = " << heat << '\n';
        diag_.push_back(Diagnostics(t, heat));
    }



    void write_diagnostics(const std::string &filename) const
    {
        std::ofstream out_file(filename, std::ios::out);
        for (const Diagnostics &d : diag_)
            out_file << d.time << '\t' << d.heat << '\n';
        out_file.close();
    }



private:

    int global(const int dir, const int a, const int b)
    {
        if (dir==0)
            return a * real_N_ + b;
        else
            return b * real_N_ + a;
    }



    void thomas(const int dir, const int nid)
    {
        std::vector<double> d_(N_);
        std::vector<double> cp_(N_);
        std::vector<double> dp_(N_);
        int i, k, k1;

        d_[0] = dir==0 ? rhs_[nid*real_N_] : rhs_[nid];
        cp_[0] = c_[0]/b_[0];
        dp_[0] = d_[0]/b_[0];
        for (int i=1; i<N_-1; i++)
        {
            k = global(dir, nid, i+1);
            d_[i]  = rhs_[k];
            cp_[i] = c_[i] / (b_[i] - a_[i] * cp_[i-1]);
            dp_[i] = (d_[i] - a_[i]*dp_[i-1]) / (b_[i] - a_[i] * cp_[i-1]);
        }
        i = N_-1;
        k = global(dir, nid, i+1);
        dp_[i] = (d_[i] - a_[i]*dp_[i-1]) / (b_[i] - a_[i] * cp_[i-1]);

        k = global(dir, nid, real_N_-2);
        rho_[k] = dp_[N_-1];
        for (int i=N_-2; i>=0; i--) {
            k  = global(dir, nid, i+1);
            k1 = global(dir, nid, i+2);
            rho_[k] = dp_[i] - cp_[i] * rho_[k1];
        }
    }



    void initialize_rho()
    {
        /*
         TODO: Subquestion 3(b):
               Paralelize the computations in
               this function with OpenMP
        */

        double bound = 0.25 * L_;

        #pragma omp parallel for
        for (int i = 1; i < real_N_-1; ++i)
        for (int j = 1; j < real_N_-1; ++j)
        {
            int k = i*real_N_ + j;
            if (std::abs((i-1)*dr_ - L_/2.) < bound && std::abs((j-1)*dr_ - L_/2.) < bound) 
               rho_[k] = 1.;
            else
               rho_[k] = 0.;
        }
    }



    double D_, L_;
    int N_, Ntot_, real_N_;
    double dr_, dt_;
    double R_;
    int rank_;
    std::vector<double> rho_, rhs_;
    std::vector<Diagnostics> diag_;
    std::vector<double> a_, b_, c_;
};



int main(int argc, char* argv[])
{

    #pragma omp parallel
    {
        #pragma omp master
        std::cout << "Running with " << omp_get_num_threads() << " threads\n";
    }

    const double D = 1;  //diffusion constant
    const double L = 1;  //domain side size
    const int N = 100;    //number of grid points per dimension
    const double dt = 1.e-4; //timestep

    Diffusion2D system(D, L, N, dt, 0);

    timer t;
    t.start();
    for (int step = 0; step < 10000; ++step) {
        system.advance();
        system.compute_diagnostics(dt * step, step);
    }
    t.stop();

    std::cout << "Timing: " << N << ' ' << t.get_timing() << '\n';

    system.write_diagnostics("diagnostics.dat");

    return 0;
}
