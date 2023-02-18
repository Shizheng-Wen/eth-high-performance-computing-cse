#include <cassert>
#include <iostream>
#include <cmath>
#include <mpi.h>
#include <vector>

struct Poisson1D
{
  //Poisson equation: d^2u/dx^2 = f(x) in [0,L] with zero boundary conditions.

  const double L;  //  domain size in x-direction
  const int N;     //  grid points in x-direction
  const double dx;  // grid spacing in x-direction
  double * u_old;   // solution vector at iteration n-1 
  double * u;       // solution vector at iteration n
  double * f;       // right hand side vector f(x,y)

  Poisson1D(const double l, const double n): L(l),N(n),dx(l/(N-1))
  {
    //Allocation of arrays
    u     = new double[N];
    u_old = new double[N];
    f     = new double[N];

    for (int i = 0; i < N; i++)
    {
      const double x  = i*dx;
      const double r2 = (x-0.5*L)*(x-0.5*L);
      u    [i] = 0.0;
      u_old[i] = 0.0;
      f    [i] = exp(-r2);
    }
  }

  double JacobiStep()
  {
    double error = 0;

    for (int i = 1; i < N-1; i++)
    {
      u[i] = 0.5*(u_old[i+1]+u_old[i-1]) - 0.5*dx*dx*f[i];
      error += std::fabs(u[i]-u_old[i]);
    }
    std::swap(u_old,u);
    error *= dx;
    return error;
  }

  void solve()
  {
    const double epsilon = 1e-6; //tolerance for Jacobi iterations
    for (int m = 0 ; m < 100000 ; m ++) //perform Jacobi iterations (up to 100000)
    {
      double curr_err = JacobiStep();
      if (m%5000==0)  std::cout << "Iteration: " << m << " error:" << curr_err << "\n";
      if (curr_err < epsilon)
      {
        std::cout << "Converged at iteration " << m << " with error: " << curr_err << std::endl; 
        break;
      }
    }
  }

  ~Poisson1D()
  {
    delete [] u;
    delete [] u_old;
    delete [] f;
  }
};

int main(int argc, char **argv)
{
  double time = -MPI_Wtime();
  const double LX = 1.0;
  const int NX = 256;
  Poisson1D poisson = Poisson1D(LX,NX);
  poisson.solve();
  time += MPI_Wtime();
  std::cout << "total time:" << time << std::endl;
  return 0;
}
