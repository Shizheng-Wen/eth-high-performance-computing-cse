#pragma once

#include <map>
#include <mpi.h>

#include "def.h"


/*
 This file contains a serial implementation of the matrix-vector product.
 It also contains an auxiliary struct that might be useful when parallelizing this computation.
 
 You are allowed to change this file however you like, in order to perform the parallel
 matrix-vector multiplication with MPI.

*/



/*
   For a given row r, we need to perform the multiplication:
    b[r] = sum(over k) A[r][k] u[k]

   Some values of k (some columns) are owned by this rank, others are not.

   For the values owned by other ranks, we need to receive u[k] from them.

   The following struct could be helpful for your solution (but you do not need to use it).
   It stores:
     1. the matrix elements A[r][k] for all r and all k that are not owned by this rank (a)
     2. all k values that are not owned by this rank (gj)
     3. all r values that have k values not owned by this rank (i)
 */
struct Sel
{
  std::vector<double> a;  // matrix elements
  std::vector<size_t> gj; // global indices of columns
  std::vector<size_t> i;  // local indices of rows
};

// Traverse matrix, multiply local elements, collect remote.
// Returns: ss[re]: selection of elements to get from remote rank re
std::map<size_t, Sel> Mul_Traverse(const Matr& A, const std::vector<double>& u, std::vector<double>& b)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::map<size_t, Sel> ss; // result

  // loop over local rows
  for (size_t row = 0; row < A.n; row++)
  {
    // loop over matrix elements in that row
    for (size_t k = A.ki[row]; k < A.ki[row + 1]; k++)
    {
      //Get the glocal column. It has a value from 0,...,Ni*Nj-1 and corresponds to a global index.
      const size_t global_column = A.gjk[k];

      //Get the rank of the global index.
      const int rank_other = GlbToRank(global_column);

      //If that rank is the same as this MPI process, do the multiplication
      if (rank_other == rank)
      {
        b[row] += A.a[k] * u[GlbToLoc(global_column)];
      } 
      else
      {
        //if not:
        //1. We save the element of the matrix that will be involved in the multiplication
        //2. We save the global column index of that element
        //3. We save the local row of that element
        ss[rank_other].a.push_back(A.a[k]);
        ss[rank_other].gj.push_back(global_column);
        ss[rank_other].i.push_back(row);
      }
    }
  }

  //After all local multiplications are performed, we return a map that maps (other) MPI ranks to:
  //1. The values of the matrix elements
  //2. The global columns
  //3. The local rows

  return ss;
}


// Multiplies matrix and vector.
std::vector<double> Mul(const Matr& A, const std::vector<double>& u)
{
  std::vector<double> b(A.n); // result

  // Traverse matrix A and perform multiplication for local elements. 
  // Also collect remote elements.
  std::map<size_t, Sel> ss = Mul_Traverse(A, u, b);

  //You need to add more stuff here, to take care of communication 
  //and parallelize the computation

  return b;
}
