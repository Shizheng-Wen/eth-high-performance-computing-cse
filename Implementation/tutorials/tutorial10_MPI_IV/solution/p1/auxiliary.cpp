#include "wave.h"

WaveEquation::WaveEquation(int a_N, int a_procs_per_dim)
{
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  procs_per_dim = a_procs_per_dim;
  Ntot = a_N;
  h = L / a_N;
  N = a_N / procs_per_dim;

  // the chosen numerical method is stable if  dt <= h/sqrt(3)
  dt = h / sqrt(3.0);

  FindCoordinates();

  origin[0] = coords[0] * N * h;
  origin[1] = coords[1] * N * h;
  origin[2] = coords[2] * N * h;
  u = new double[(N + 2) * (N + 2) * (N + 2)];
  for (int i0 = 0; i0 < N; i0++)
  {
    double x0 = origin[0] + i0 * h + 0.5 * h;
    for (int i1 = 0; i1 < N; i1++)
    {
      double x1 = origin[1] + i1 * h + 0.5 * h;
      for (int i2 = 0; i2 < N; i2++)
      {
        double x2 = origin[2] + i2 * h + 0.5 * h;
        u[(i0 + 1) * (N + 2) * (N + 2) + (i1 + 1) * (N + 2) + (i2 + 1)] =
            Initial_Condition(x0, x1, x2);
      }
    }
  }

  u_old = new double[(N + 2) * (N + 2) * (N + 2)];
  u_new = new double[(N + 2) * (N + 2) * (N + 2)];

  for (int i0 = 1; i0 <= N; i0++)
    for (int i1 = 1; i1 <= N; i1++)
      for (int i2 = 1; i2 <= N; i2++)
      {
        int m = i2 + i1 * (N + 2) + i0 * (N + 2) * (N + 2);
        u_new[m] = u[m];
        u_old[m] = u[m];
        // assuming that u_old = u is equivalent to du/dt(t=0) = 0
      }

  aux = dt * dt / h / h;
}

WaveEquation::~WaveEquation()
{
  delete[] u;
  delete[] u_old;
  delete[] u_new;
}

double WaveEquation::Initial_Condition(double x0, double x1, double x2)
{
  double r =
      (x0 - 0.5) * (x0 - 0.5) + (x1 - 0.5) * (x1 - 0.5) + (x2-0.5)*(x2-0.5);
  return exp(-r / 0.1);
}


// do not change this function
void WaveEquation::UpdateGridPoint(int i0, int i1, int i2)
{
  int m = i0 * (N + 2) * (N + 2) + i1 * (N + 2) + i2;
  int ip1 = (i0 + 1) * (N + 2) * (N + 2) + (i1) * (N + 2) + (i2);
  int im1 = (i0 - 1) * (N + 2) * (N + 2) + (i1) * (N + 2) + (i2);
  int jp1 = (i0) * (N + 2) * (N + 2) + (i1 + 1) * (N + 2) + (i2);
  int jm1 = (i0) * (N + 2) * (N + 2) + (i1 - 1) * (N + 2) + (i2);
  int kp1 = (i0) * (N + 2) * (N + 2) + (i1) * (N + 2) + (i2 + 1);
  int km1 = (i0) * (N + 2) * (N + 2) + (i1) * (N + 2) + (i2 - 1);
  u_new[m] =
      2.0 * u[m] - u_old[m] +
      aux * (u[ip1] + u[im1] + u[jp1] + u[jm1] + u[kp1] + u[km1] - 6.0 * u[m]);
}
