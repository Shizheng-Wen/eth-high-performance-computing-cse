#pragma once

#include <map>
#include <mpi.h>

#include "def.h"


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
// Returns:
// ss[re]: selection of elements to get from remote rank re
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


//The following function takes 'ss' as input. 
//ss[other_rank] contains the global indices gj of vector u that this rank needs from other_rank.
//This function will send these indices to other_rank, so that other_rank knows what elements of u
//it needs to send to this rank.
std::map<size_t, std::vector<size_t>> ExchangeIndex(const std::map<size_t, Sel>& ss)
{
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<size_t> messages(size, 0); // messages[i] is the number of messages that rank i will receive
  for (auto& p : ss) // loop over ranks
  {
    const size_t other_rank = p.first;
    ++messages[other_rank];
  }
  // sum over all ranks
  MPI_Allreduce(MPI_IN_PLACE, messages.data(), messages.size(), MPI_UINT64_T, MPI_SUM, MPI_COMM_WORLD);
  const size_t number_of_messages = messages[rank];


  std::map<size_t, std::vector<size_t>> indices_needed; // result

  int tag = 0;

  std::vector<MPI_Request> requests(ss.size());

  //I am rank 'rank' and I need the values of u with indices s[other_rank].gj from every 'other_rank'.
  //Here I send these indices to every 'other_rank', so that they know what values they need to send
  //me later.
  int k = 0;
  for (auto& p : ss)
  {
    const int other_rank = p.first;
    auto & indices       = p.second.gj;
    MPI_Isend(indices.data(), indices.size(), MPI_UINT64_T, other_rank, tag, MPI_COMM_WORLD, &requests[k]);
    ++k;
  }

  //I am rank 'rank' and I expect to receive 'number_of_messages' messages.
  //Each message will tell me what indices the other ranks need from me.
  //However, I do not know the number of the indices (the message size), so I need to find that
  //first with MPI_Probe and MPI_Get_count
  for (size_t i = 0; i < number_of_messages; ++i)
  {
    MPI_Status status;

    MPI_Probe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status); //check any of the incoming messages
    int other_rank = status.MPI_SOURCE;

    int message_size;
    MPI_Get_count(&status, MPI_UINT64_T, &message_size); //get size of that incoming message

    //receive the message
    indices_needed[other_rank].resize(message_size);
    MPI_Recv(indices_needed[other_rank].data(), indices_needed[other_rank].size(), MPI_UINT64_T, other_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  MPI_Waitall(requests.size(), requests.data(), MPI_STATUSES_IGNORE);

  //indices_needed[other_rank] now contains the indices that I (this rank) need to send to other_rank 
  //in the next function I will send the values u[indices_needed[other_rank]] to other_rank, so that
  //it can do the multiplication
  return indices_needed;
}


//I am rank 'rank' and I now know that other_rank needs me to send the values of u{indices_needed[other_rank]}
//I also know that I need to receive ss.gj[other_rank] for other_rank
//and use what I receive to do the multiplication with ss.a[other_rank] found in rows ss.i[other_rank]
void ExchangeValues(const std::map<size_t, Sel>& ss, const std::map<size_t, std::vector<size_t>>& indices_needed, const std::vector<double>& u, std::vector<double>& b)
{
  std::vector<MPI_Request> requests(indices_needed.size());

  std::vector<std::vector<double>> send_buffer(indices_needed.size()); // buffer for values to send

  int tag = 1;

  // send values
  size_t k = 0;
  for (auto& p : indices_needed)
  {
    int other_rank = p.first;

    for (auto gj : p.second)
    {
      send_buffer[k].push_back(u[GlbToLoc(gj)]); //I need to send u{indices_needed[other_rank]} to other_rank, so I put it in a buffer here
    }

    MPI_Isend(send_buffer[k].data(), send_buffer[k].size(), MPI_DOUBLE, other_rank, tag, MPI_COMM_WORLD, &requests[k]);
    ++k;
  }

  // receive values
  for (auto& p : ss)
  {
    //I will now receive what I need from rank 'other_rank'
    int other_rank = p.first;

    auto& s = p.second;

    std::vector<double> v(s.gj.size()); //I need gj.size elements from other rank

    MPI_Recv(v.data(), v.size(), MPI_DOUBLE, other_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // do the multiplication with what I received
    for (size_t k = 0; k < v.size(); ++k)
    {
      b[s.i[k]] += s.a[k] * v[k];
    }
  }

  MPI_Waitall(requests.size(), requests.data(), MPI_STATUSES_IGNORE);
}


// Multiplies matrix and vector.
std::vector<double> Mul(const Matr& A, const std::vector<double>& u)
{
  std::vector<double> b(A.n); // result

  // Traverse matrix A and perform multiplication for local elements. 
  // Also collect remote elements.
  std::map<size_t, Sel> ss = Mul_Traverse(A, u, b);

  // exchange indices
  std::map<size_t, std::vector<size_t>> indices_needed = ExchangeIndex(ss);

  MPI_Barrier(MPI_COMM_WORLD);

  // exchange values, append to result
  ExchangeValues(ss, indices_needed, u, b);

  return b;
}
