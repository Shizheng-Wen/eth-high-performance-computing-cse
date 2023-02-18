#include <cassert>
#include <iostream>
#include <cmath>
#include <omp.h>
#include <vector>

struct Poisson2D
{
  //Poisson equation: d^2u/dx^2 + d^2u/dy^2 = f(x,y) in [0,Lx]x[0,Ly] with zero boundary conditions.

  const double Lx;  //  domain size in x-direction
  const double Ly;  //  domain size in y-direction
  const int Nx;     //  grid points in x-direction
  const int Ny;     //  grid points in y-direction
  const double dx;  // grid spacing in x-direction
  const double dy;  // grid spacing in y-direction
  double * u_old;   // solution vector at iteration n-1 
  double * u;       // solution vector at iteration n
  double * f;       // right hand side vector f(x,y)
  double error;     // Jacobi iterations error (Em)

  Poisson2D(const double lx, const double ly, const double nx, const double ny): Lx(lx),Ly(ly),Nx(nx),Ny(ny),dx(lx/(Nx-1)),dy(ly/(Ny-1))
  {
    //Allocation of arrays
    u     = new double[Nx*Ny];
    u_old = new double[Nx*Ny];
    f     = new double[Nx*Ny];

    for (int iy = 0; iy < Ny; iy++)
    for (int ix = 0; ix < Nx; ix++)
    {
      const double x  = ix*dx;
      const double y  = iy*dx;
      const double r2 = (x-0.5*Lx)*(x-0.5*Lx)+(y-0.5*Ly)*(y-0.5*Ly);
      u    [iy*Nx+ix] = 0.0;
      u_old[iy*Nx+ix] = 0.0;
      f    [iy*Nx+ix] = exp(-r2);
    }
  }

  double JacobiStep()
  {
    error = 0;

    for (int iy = 1; iy < Ny-1; iy++)
    for (int ix = 1; ix < Nx-1; ix++)
    {
      u[iy*Nx+ix] = ((u_old[iy*Nx+ix+1] + u_old[iy*Nx+ix-1])/dx/dx + (u_old[(iy+1)*Nx+ix] + u_old[(iy-1)*Nx+ix])/dy/dy - f[iy*Nx+ix])/(2.0/dx/dx + 2.0/dy/dy);
      error += std::fabs(u[iy*Nx+ix]-u_old[iy*Nx+ix]);
    }

    std::swap(u_old,u);
    error *= dx*dy;

    return error;
  }

  void solve()
  {
    const double epsilon = 1e-6; //tolerance for Jacobi iterations
    for (int m = 0 ; m < 10000 ; m ++) //perform Jacobi iterations (up to 100000)
    {
        double curr_err = JacobiStep();
        if (m%1000==0)  std::cout << m << " " << curr_err << "\n";
        if (curr_err < epsilon) break;
    }
  }

  ~Poisson2D()
  {
    delete [] u;
    delete [] u_old;
    delete [] f;
  }
};

int main(int argc, char **argv)
{
  double time = -omp_get_wtime();
  const double LX = 2.0;
  const double LY = 1.0;
  const int NX = 1024;
  const int NY = 512;
  Poisson2D poisson = Poisson2D(LX,LY,NX,NY);
  poisson.solve();
  time += omp_get_wtime();
  std::cout << "total time:" << time << std::endl;
  return 0;
}
