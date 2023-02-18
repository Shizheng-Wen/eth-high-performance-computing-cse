#include <cassert>
#include <mpi.h>

#include "def.h"
#include "mul.h"

// YOU DO NOT NEED TO MODIFY THIS FILE

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  assert(size == NBi * NBj);

  // Laplacian
  Matr A = GetLapl(rank);

  // Initial vector
  std::vector<double> u;
  for (size_t Iloc = 0; Iloc < L; ++Iloc) //loop over local indices
  {
    //get global index from local index
    const size_t Iglob = LocToGlb(Iloc, rank);

    //get global (i,j) coordinates from global index
    const auto ij = GlbToCoord(Iglob);

    //get (xi,xj) position in space and set initial condition
    const double xi = double(ij[0]) / Ni;
    const double xj = double(ij[1]) / Nj;
    const double dxi = xi - 0.5;
    const double dxj = xj - 0.5;
    const double r = 0.2;
    u.push_back(dxi*dxi + dxj*dxj < r*r ? 1. : 0.);
  }

  Dump(u, GetName(0));

  const double coef = 0.25; // = dt / h^2 , must be <= 0.25 for stability
  const size_t nt = 10; // number of time steps
  for (size_t t = 0; t < nt; ++t)
  {
    std::vector<double> du = Mul(A, u);
    for (size_t k = 0 ; k < u.size(); k++)
    {
      u[k] += coef * du[k];
    }
  }

  Dump(u, GetName(1));

  MPI_Finalize();
}
