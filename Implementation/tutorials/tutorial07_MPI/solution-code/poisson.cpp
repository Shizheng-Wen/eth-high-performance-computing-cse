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

  int rank,size;
  int istart,iend;
  int myN;
  double u_left, u_right;

  Poisson1D(const double l, const double n): L(l),N(n),dx(l/(N-1))
  {
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    myN = N/size;

    istart =  rank   *myN;
    iend   = (rank+1)*myN;

    //Allocation of arrays
    u     = new double[myN];
    u_old = new double[myN];
    f     = new double[myN];

    for (int i = istart; i < iend; i++)
    {
      const int iloc = i - istart;
      const double x  = i*dx;
      const double r2 = (x-0.5*L)*(x-0.5*L);
      u    [iloc] = 0.0;
      u_old[iloc] = 0.0;
      f    [iloc] = exp(-r2);
    }
  }

  double JacobiStep()
  {
    int tag = 666;

    double error = 0;

    int  left_rank = rank - 1;
    int right_rank = rank + 1;

    if (left_rank   <    0)
    {
      u_left  = 0;
      left_rank = MPI_PROC_NULL;
    }
    if (right_rank  >=  size)
    {
      u_right  = 0;
      right_rank = MPI_PROC_NULL;
    }

    //non-blocking solution
    //MPI_Request request[4];
    //MPI_Irecv(&u_left      , 1, MPI_DOUBLE, left_rank , tag, MPI_COMM_WORLD, &request[0]);
    //MPI_Irecv(&u_right     , 1, MPI_DOUBLE, right_rank, tag, MPI_COMM_WORLD, &request[1]);
    //MPI_Isend(&u_old[0]    , 1, MPI_DOUBLE, left_rank , tag, MPI_COMM_WORLD, &request[2]);
    //MPI_Isend(&u_old[myN-1], 1, MPI_DOUBLE, right_rank, tag, MPI_COMM_WORLD, &request[3]);

    //blocking solution
    MPI_Sendrecv(&u_old[0]    , 1, MPI_DOUBLE,  left_rank, tag,
                 &u_left      , 1, MPI_DOUBLE,  left_rank, tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    MPI_Sendrecv(&u_old[myN-1], 1, MPI_DOUBLE, right_rank, tag,
                 &u_right     , 1, MPI_DOUBLE, right_rank, tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    for (int i = istart + 1; i < iend - 1; i++)
    {
      const int iloc = i - istart;
      u[iloc] = 0.5*(u_old[iloc+1]+u_old[iloc-1]) - 0.5*dx*dx*f[iloc];
      error += std::fabs(u[iloc]-u_old[iloc]);
    }

    //non-blocking solution: we can wait for commmunication to complete here,
    //after the inner points are computed.
    //This is faster than waiting for communication (in the blocking solution)
    //and then computing all the points.
    //MPI_Waitall(4,request,MPI_STATUSES_IGNORE);

    if (istart != 0)
    {
      const int i = istart;
      const int iloc = i - istart;
      u[iloc] = 0.5*(u_old[iloc+1]+u_left) - 0.5*dx*dx*f[iloc];
      error += std::fabs(u[iloc]-u_old[iloc]);
    }
    if (iend-1 != N-1)
    {
      const int i = iend-1;
      const int iloc = i - istart;
      u[iloc] = 0.5*(u_right+u_old[iloc-1]) - 0.5*dx*dx*f[iloc];
      error += std::fabs(u[iloc]-u_old[iloc]);
    }
    std::swap(u_old,u);
    error *= dx;
    MPI_Allreduce(MPI_IN_PLACE,&error,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
    return error;
  }

  void solve()
  {
    const double epsilon = 1e-8; //tolerance for Jacobi iterations
    for (int m = 0 ; m < 10000000 ; m ++) //perform Jacobi iterations (up to 10000000)
    {
      double curr_err = JacobiStep();
      if (m%10000==0 && rank == 0)  std::cout << "Iteration: " << m << " error:" << curr_err << "\n";
      if (curr_err < epsilon)
      {
        if (rank == 0)
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
  MPI_Init(&argc,&argv);
  double time = -MPI_Wtime();
  const double L = 1.0;
  const int N = 4096;
  Poisson1D poisson = Poisson1D(L,N);
  poisson.solve();
  time += MPI_Wtime();
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if (rank == 0)
    std::cout << "total time:" << time << std::endl;
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
