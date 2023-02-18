#include <cassert>
#include <chrono>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>

#include <omp.h>

#include "utils.h"


// Interface for LAPACK routines.
// On Euler, you must load the MKL library:
// $ module load mkl
#include <mkl_lapack.h>



int main (int argc, char** argv)
{

  // DATA PARAMETERS
  const int D = 2;  // Data dimension
  const int N = 250; // Number of samples
  const int num_comp = 2; // Number of principal components
  std::string data_path = "data.txt"; // Data path

  ///////////////////////////////////////////////////////////////////////////
  // Reading data. The data dimension is N x D.  The returned pointer points
  // to the data in row-major order. That is, if (i,j) corresponds to
  // the row and column index, respectively, you access the data with
  // data_{i,j} = data[i*D + j], where 0 <= i < N and 0 <= j < D.
  // The pointer points to a total memory allocation of N * D doubles.
  // In this example, i corresponds to the sample number n and j to the data
  // dimension (equivalently data[n*D+d]).
  ///////////////////////////////////////////////////////////////////////////

  std::cout << "Loading dataset from: " << data_path << std::endl;
  double* data = utils::loadDataset(data_path, N, D);


  /////////////////////////////////////////////////////////////////////////
  // PCA IMPLEMENTATION
  // 1. Transpose data (save them to new array)
  // 2. Compute mean and standard deviation of your data
  // 3. Normalize the data
  // 4. Build the covariance matrix
  // 5. Compute the eigenvalues and eigenvectors of the covariance matrix.
  //    Use LAPACK here.
  // 6. Extract the principal components and save them
  // 7. Reduce the dimensionality of the data by applying the compresion
  // 8. Report the compression ratio
  // 9. Reconstruct a reference image from its compressed form and save
  //    it in a .txt file
  /////////////////////////////////////////////////////////////////////////

  double time_start;
  double time_end;


  /////////////////////////////////////////////////////////////////////////
  // 1. Transpose data for efficient computation of mean over samples
  double* data_T = new double[N*D];
  utils::transposeData(data_T, data, N, D);


  /////////////////////////////////////////////////////////////////////////
  // 2. Compute mean and standard deviation of your data
  time_start = omp_get_wtime();
  double* data_mean = new double[D];
  double* data_std = new double[D];
  utils::computeMean(data_mean, data_T, N, D);
  utils::computeStd(data_std, data_mean, data_T, N, D);

  // Writing the mean and standard deviation to a file
  utils::writeRowMajorMatrixToFile("./results/mean.txt", data_mean, 1, D);
  utils::writeRowMajorMatrixToFile("./results/std.txt", data_std, 1, D);

  // Time tracking
  time_end = omp_get_wtime();
  std::cout << "MEAN/STD TIME = " << time_end-time_start << " seconds\n";


  /////////////////////////////////////////////////////////////////////////
  // 3. Normalize the data
  utils::centerDataColMajor(data_T, data_mean, N, D);


  /////////////////////////////////////////////////////////////////////////
  // 4. Build the covariance matrix
  time_start = omp_get_wtime();
  double* data_cov = new double[D*D];
  utils::constructCovariance(data_cov, data_T, N, D);

  time_end = omp_get_wtime();
  std::cout << "COVARIANCE-MATRIX TIME = " << time_end-time_start << " seconds\n";


  /////////////////////////////////////////////////////////////////////////
  // 5. Compute the eigenvalues and eigenvectors of the covariance matrix.
  //    Use LAPACK here.
  time_start = omp_get_wtime();

  // Consult the interface given in the link
  // https://netlib.org/lapack/explore-html/d2/d8a/group__double_s_yeigen_ga442c43fca5493590f8f26cf42fed4044.html#ga442c43fca5493590f8f2%20%20%20%206cf42fed4044

  char jobz = 'V'; // Compute both eigenvalues and orthonormal eigenvectors
  char uplo = 'U'; // Upper triangular matrix
  int info, lwork;

  double* W = new double[D]; // Eigenvalues
  double* work = new double[2];

  // first call to dsyev_() with lwork = -1 to determine the optimal workspace (cheap call)
  lwork=-1;

  // First call to dsyen to determine optimal size of work array 
  // first call dsyev, determine optimal size of work array
  dsyev_(&jobz, &uplo, &D, data_cov, &D, W, work, &lwork, &info);

  lwork = (int)work[0];
  delete[] work;
  // allocate optimal workspace
  work = new double[lwork];

  // Second call to dsyen
  dsyev_(&jobz, &uplo, &D, data_cov, &D, W, work, &lwork, &info);

  // Upon completion data_cov contains the orthonormal eigenvectors of the covariance matrix stored in ROW major format: data_cov(j,k)=data_cov[j*D+k]
  // The eigenvalues are returned in ASCENTING order

  time_end = omp_get_wtime();
  std::cout << "DSYEV TIME = " << time_end-time_start << " seconds\n";
  // clean-up
  delete[] work;


  /////////////////////////////////////////////////////////////////////////
  // 6. Extract the principal components & eigenvalues and save them
  utils::reverseArray(W, D);
  utils::writeRowMajorMatrixToFile("./results/eig.txt", W, 1, D);
  delete[] W;

  double* V = new double[num_comp * D];
  utils::getEigenvectors(V, data_cov, num_comp, D);
  delete[] data_cov;

  // save PCA components
  utils::writeRowMajorMatrixToFile("./results/components.txt", V, num_comp, D);


  /////////////////////////////////////////////////////////////////////////
  // 7. Reduce the dimensionality of the data by applying the compresion
  time_start = omp_get_wtime();

  double* data_reduced = new double[N * num_comp]; // saved in normal row major order (not transpose!)
  utils::reduceDimensionality(data_reduced, V, data_T, N, D, num_comp);

  // save to file
  utils::writeRowMajorMatrixToFile("./results/data_reduced.txt", data_reduced, num_comp, N);

  time_end = omp_get_wtime();
  std::cout << "PCREDUCED TIME = " << time_end-time_start << " seconds\n";


  /////////////////////////////////////////////////////////////////////////
  // 8. Report the compression ratio

  const double dim_old = N * D;
  // For every component (num_comp) we save the basis (D), and the normalization data (2*D)

  // In order to compress a dataset, we need:
  // The normalization data (2*D)
  // The compressed dataset (N*num_comp)
  // The PCA basis (num_comp*D)
  const double dim_comp = num_comp * (D + N) + 2*D;
  // Report the compression ratio
  std::cout << "COMPRESSION RATIO = " << dim_old/dim_comp << std::endl;


  /////////////////////////////////////////////////////////////////////////
  // 9. Reconstruct the data from their compressed form and save them in a
  // .txt file
  double* data_rec = new double[N * D];
  utils::reconstructDatasetRowMajor(data_rec, V, data_reduced, N, D, num_comp);

  /////////////////////////////////////////////////////////////////////////
  // Un-Normalize the data
  utils::inverseCenterDatasetRowMajor(data_rec, data_mean, N, D);
  utils::writeRowMajorMatrixToFile("./results/data_reconstructed.txt", data_rec, N, D);


  // Clean-up
  delete[] data_rec;
  delete[] data_reduced;
  delete[] V;
  delete[] data_std;
  delete[] data_mean;
  delete[] data_T;
  delete[] data;
  return 0;
}

