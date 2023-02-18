#pragma once

#include <mpi.h>
#include <cstddef>
#include <vector>
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>

/*
  The grid contains Ni x Nj grid points.

  It is decomposed to NBi x NBj blocks and each MPI rank is assigned one block.

  Each block contains Ni/NBi x Nj/NBj = Bi x Bj grid points.
*/

#define Ni  64  // total number of grid points in i-direction
#define Nj  128 // total number of grid points in j-direction

#define NBi 1   // number of blocks in i-direction
#define NBj 4   // number of blocks in j-direction



/***** YOU DO NOT NEED TO MODIFY ANYTHING BELOW THIS LINE *****/



const size_t Bi = Ni / NBi; // size of each block in i-direction
const size_t Bj = Nj / NBj; // size of each block in j-direction

const size_t L = Bi * Bj; // number of grid points per rank (each rank is assigned one block)

static_assert(Nj % NBj == 0 && Ni % NBi == 0, "Grid size not divisible by number of blocks");


/*
  Each grid point is associated with:

  1. Global coordinates (i,j), where i=0,...,Ni-1 and j=0,...,Nj-1
  2. Global index Iglob = 0,...,Ni*Nj-1
  3. Local coordinates (iloc,jloc), where iloc=0,...,Bi-1 and jloc=0,...,Bj-1
  4. Local index Iloc = 0,...,Bi*Bj-1

  Example: 
          Ni = 64, Nj  = 96
          NBi = 2, NBj =  3 (total of 6 blocks, each block contains 32 x 32 grid points)

          Grid point marked with x has:
               (i   ,j   ) = 0
               (iloc,jloc) = 0
               Iglob = 0
               Iloc  = 0

          Grid point marked with y has:
               (i   ,j   ) = (Ni/2-1,Nj/3-1)
               (iloc,jloc) = (0     ,Bj-1)
               Iglob = 3*(Bi*Bj)+Bj-1
               Iloc  = Bj-1

          Grid point marked with z has:
               (i   ,j   ) = (Ni-1,Nj-1)
               (iloc,jloc) = (Bi-1,Bj-1)
               Iglob = Ni*Nj-1
               Iloc  = Bi*Bj-1
  ^ (i)
  |
  |
  -------------------------------
  |         |         |        z|
  |         |         |         |
  |  rank 3 |  rank 4 |  rank 5 |
  |        y|         |         |
  -------------------------------
  |         |         |         |
  |         |         |         |
  |  rank 0 |  rank 1 |  rank 2 |
  |x        |         |         |
  -------------------------------------> (j)

*/

//The functions below convert between local and global coordinates and indices:

// Converts coordinates to global index.
size_t CoordToGlb(const size_t i, const size_t j)
{
  return Nj * i + j;
}

// Converts global index to coordinates.
std::array<size_t, 2> GlbToCoord(const size_t Iglob)
{
  const size_t j = Iglob % Nj;
  const size_t i = Iglob / Nj;
  return {i,j};
}

// Converts global index to rank.
size_t GlbToRank(const size_t Iglob)
{
  const auto ij = GlbToCoord(Iglob);
  const size_t block_i = ij[0] / Bi;
  const size_t block_j = ij[1] / Bj;
  return NBj * block_i + block_j;
}

// Converts global index to local.
size_t GlbToLoc(const size_t Iglob)
{
  const auto ij = GlbToCoord(Iglob);
  const size_t iloc = ij[0] % Bi;
  const size_t jloc = ij[1] % Bj;
  return Bj * iloc + jloc;
}

// Converts local index to global.
size_t LocToGlb(const size_t Iloc, const size_t rank)
{
  const size_t block_i = rank / NBj;
  const size_t block_j = rank % NBj;
  const size_t iloc = Iloc / Bj;
  const size_t jloc = Iloc % Bj;
  const size_t i = block_i * Bi + iloc;
  const size_t j = block_j * Bj + jloc;
  return CoordToGlb(i,j);
}

/*
  Distributed matrix, stored in CSR format.
 */
struct Matr
{
  std::vector<double> a;         // data, size nnz, a[k], k=0,...,nnz-1

  size_t n = 0;                  // number of (local) rows that this MPI rank owns

  std::vector<size_t> ki = {0};  // the i-th local row starts at a[k0] and finishes at a[k1]
                                 // where k0 = ki[i] and k1=ki[i+1]

  std::vector<size_t> gjk;       // each element a[k] is located at global column gik[k], k=0,...,nnz-1
};

/* 
   Returns (distributed) Laplacian matrix. 

   The full matrix is an (Ni x Nj) x (Ni x Nj) matrix.

   Each rank owns (Ni x Nj)/(total ranks) rows and all (Ni x Nj) columns of the matrix.
 */
Matr GetLapl(const size_t rank)
{
  Matr A;

  // loop over local indices
  for (size_t Iloc = 0; Iloc < L; ++Iloc)
  {
    //get global index from local index
    const size_t Iglob = LocToGlb(Iloc, rank);

    //get global (i,j) coordinates from global index
    auto ij = GlbToCoord(Iglob);
    const size_t i = ij[0];
    const size_t j = ij[1];

    assert(CoordToGlb(ij[0], ij[1]) == Iglob);

    //get global coordinates of points that are adjacent to (i,j)
    const size_t im = (i + Ni - 1) % Ni; //(i-1,j  )
    const size_t ip = (i + 1     ) % Ni; //(i+1,j  )
    const size_t jm = (j + Nj - 1) % Nj; //(i  ,j-1)
    const size_t jp = (j + 1     ) % Nj; //(i  ,j+1)

    //convert global coordinates to global indices
    const size_t Iglob_im = CoordToGlb(im, j );
    const size_t Iglob_ip = CoordToGlb(ip, j );
    const size_t Iglob_jm = CoordToGlb(i , jm);
    const size_t Iglob_jp = CoordToGlb(i , jp);

    //global index Iglob [for global coordinates (i,j)] is assigned a value of -4.
    A.a.push_back(-4.);
    A.gjk.push_back(Iglob);

    //global index Iglob [for global coordinates (i-1,j)] is assigned a value of 1.
    A.a.push_back(1.);
    A.gjk.push_back(Iglob_im);

    //global index Iglob [for global coordinates (i+1,j)] is assigned a value of 1.
    A.a.push_back(1.);
    A.gjk.push_back(Iglob_ip);

    //global index Iglob [for global coordinates (i,j-1)] is assigned a value of 1.
    A.a.push_back(1.);
    A.gjk.push_back(Iglob_jm);

    //global index Iglob [for global coordinates (i,j+1)] is assigned a value of 1.
    A.a.push_back(1.);
    A.gjk.push_back(Iglob_jp);

    //number of rows of matrix A is increased by one
    ++A.n;

    //The row that we just added to matrix A starts at index equal to the current a.size()
    A.ki.push_back(A.a.size());
  }

  return A;
}


//The functions below are used to save the solution vector to a file. They use MPI I/O calls.

std::string GetName(size_t i)
{
  std::stringstream n;
  n << "u_" << std::setfill('0') << std::setw(3) << i << ".dat";
  return n.str();
}

// Writes vector to file using MPI IO.
void Dump(const std::vector<double>& u, const std::string filename)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Print vector to stream.
  std::stringstream st;
  for (size_t i = 0; i < L; ++i)
  {
    auto gi = LocToGlb(i, rank);
    auto xy = GlbToCoord(gi);
    st << double(xy[1] + 0.5) / Nj << " " << double(xy[0] + 0.5) / Nj << " " << u[i] << "\n";
  }
  std::string s = st.str();

  MPI_File file;

  MPI_Offset ls = s.size();  // length on current rank
  MPI_Offset offset = 0;
  MPI_Exscan(&ls, &offset, 1, MPI_OFFSET, MPI_SUM, MPI_COMM_WORLD);

  MPI_File_open(MPI_COMM_WORLD, filename.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

  MPI_File_write_at_all(file, offset, s.data(), s.size(), MPI_CHAR, MPI_STATUS_IGNORE);

  MPI_File_close(&file);
}
