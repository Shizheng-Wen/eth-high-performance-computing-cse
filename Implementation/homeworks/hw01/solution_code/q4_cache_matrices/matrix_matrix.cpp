#include <cassert>
#include <chrono>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>

// Interface for LAPACK routines.
// On Euler, you must load the MKL library:
// $ module load mkl
#include <mkl.h>

//Global variable to check if every calculation of C is right
double C_tot_lapack;


void AB(double* A, double*  B_row, double*  C, size_t N){
    for (size_t i = 0; i < N*N; i++)
        C[i] = 0;

    // TODO: Question 4c: Straightforward matrix-matrix multiplication
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
            for (size_t k = 0; k < N; k++)
                C[i * N + j] += A[i * N + k] * B_row[k * N + j];
}

void AB_block_row(double*  A, double*  B_row,
                  double*  C, size_t N, size_t blockSize){
    for (size_t i = 0; i < N*N; i++)
        C[i] = 0;

    // TODO: Question 4c: Block matrix-matrix multiplication - B in row major
    for (size_t K = 0; K < N; K += blockSize)
        for (size_t I = 0; I < N; I += blockSize)
            for (size_t J = 0; J < N; J += blockSize)
                for (size_t i = I; i < I + blockSize; i++)
                    for (size_t j = J; j < J + blockSize; j++)
                        for (size_t k = K; k < K + blockSize; k++)
                            C[i * N + j] += A[i * N + k] * B_row[k * N + j];
}

void AB_block_col(double*  A, double*  B_col,
                  double*  C, size_t N, size_t blockSize){
    for (size_t i = 0; i < N*N; i++)
        C[i] = 0;

    // TODO: Question 4c: Block matrix-matrix multiplication - B in column major
    for (size_t K = 0; K < N; K += blockSize)
        for (size_t I = 0; I < N; I += blockSize)
            for (size_t J = 0; J < N; J += blockSize)
                for (size_t i = I; i < I + blockSize; i++)
                    for (size_t j = J; j < J + blockSize; j++)
                        for (size_t k = K; k < K + blockSize; k++)
                            C[i * N + j] += A[i * N + k] * B_col[k + N * j];
}

void AB_lapack(double* A, double*  B_row, double*  C, size_t N){
    //cblas_dgemm: C = alpha*A*B + beta*C
    //alpha, beta: double precision scalars
    //A,B,C: double precisions matrices of dim: NxN

    double ALPHA = 1.0; 
    double BETA = 0.0;
    int LDA = N;
    int LDB = N;
    int LDC = N;

    // TODO: call dgemm_() and fill in the parameters as shown on this pdf:
    // https://www.intel.com/content/dam/develop/external/us/en/documents/mkl-2018-developer-guide-win.pdf (page 43)
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, ALPHA, A, LDA, B_row, LDB, BETA, C, LDC);
}

double benchmark_AB(double* A, double* B, size_t N, size_t mode,
                    size_t blockSize, size_t Ns)
{
    double times = 0;

    if ((mode == 2 or mode == 3) && N % blockSize != 0) {
        fprintf(stderr,
                "Error: the size of the matrix (%zu) should be divided by the "
                "blockSize variable (%zu).\n",
                N, blockSize);
        exit(1);
    }

    double* C = (double*) malloc(N*N*sizeof(double));

    std::string mode_str;

    for (size_t i = 0; i < Ns; i++) {
        auto t1 = std::chrono::system_clock::now();
        // TODO: Call the function to be benchmarked
        if (mode == 1){
            AB(A, B, C, N);
            mode_str = "Straightforward matrix-matrix multiplication";
        }else if (mode == 2){
            AB_block_row(A, B, C, N, blockSize);
            mode_str = "Block matrix-matrix multiplication - B in row major";
        }else if (mode == 3){
            AB_block_col(A, B, C, N, blockSize);
            mode_str = "Block matrix-matrix multiplication - B in col major";
        }else if (mode == 4){
            AB_lapack(A, B, C, N);
            mode_str = "Lapack";
        }
        auto t2 = std::chrono::system_clock::now();
        times += std::chrono::duration<double>(t2 - t1).count();
    }

    double C_tot_sum = 0.0;
    for(size_t i = 0; i < N*N; ++i){
        C_tot_sum += C[i]/(N*N);
    }

    
    if(mode == 4){
        C_tot_lapack = C_tot_sum;
    }else{
        double C_error = std::abs(C_tot_sum - C_tot_lapack);
        if(C_error > 1e-20){
            std::cout << "ERROR: calculation of matrix C from: (" << mode_str << ") does not give the same result as Lapack!\n The error is: " << C_error << "\n";
            exit(1);
        }
    }

    fprintf(stderr, "Done in total %9.4fs  --  average %9.4fs\n", times,
            times / Ns);
    fprintf(stderr, "Total sum of C = %9.4f\n", C_tot_sum);

    return times / Ns;
}

int main()
{
    std::vector<size_t> matrixSize{512, 1024, 2048};
    size_t M = matrixSize.size();
    std::vector<size_t> Ns{10, 5, 1};
    assert(Ns.size() >= matrixSize.size());

    std::vector<size_t> blockSize;
    for (int b = 2; b <= 512; b *= 2) {
        blockSize.push_back(b);
    }
    size_t Bs = blockSize.size();

    std::vector<double> times1(M, 0);
    std::vector<std::vector<double>> times2(Bs, std::vector<double>(M, 0));
    std::vector<std::vector<double>> times3(Bs, std::vector<double>(M, 0));
    std::vector<double> times4(M, 0);

    double *A = (double*) malloc(1);
    double *B_row = (double*) malloc(1); 
    double *B_col = (double*) malloc(1);

    for (size_t m = 0; m < M; m++) {

        fprintf(stderr, "Working with matrices of size %zu\n", matrixSize[m]);
        fprintf(stderr, "---------------------------------------------\n");

        size_t N = matrixSize[m];

        // TODO: Randomly initialize matrices
        //       store A and B_row as row major and B_col as column major
        A = (double*) malloc(N*N*sizeof(double));
        B_row = (double*) malloc(N*N*sizeof(double));
        B_col = (double*) malloc(N*N*sizeof(double));
        for (size_t i = 0; i < N; i++){
            for (size_t j = 0; j < N; j++) {
                A[i * N + j] = 1.1 * i + j;
                B_row[i * N + j] = 1.05 * i + j;
                B_col[i + N * j] = 1.05 * i + j;
            }
        }
        fprintf(stderr, "---------------------------------------------\n");

        fprintf(stderr, "Start C=A*B (LAPACK).\n");
        times4[m] = benchmark_AB(A, B_row, N, 4, 0, Ns[m]);

        fprintf(stderr, "---------------------------------------------\n");

        fprintf(stderr, "Start C=A*B (non optimized).\n");
        times1[m] = benchmark_AB(A, B_row, N, 1, 0, Ns[m]);

        fprintf(stderr, "---------------------------------------------\n");

        for (size_t b = 0; b < Bs; b++) {
            fprintf(stderr,"Start C=A*B (optimized, row major, block size=%zu).\n",blockSize[b]);
            times2[b][m] = benchmark_AB(A, B_row, N, 2, blockSize[b], Ns[m]);
        }

        fprintf(stderr, "---------------------------------------------\n");

        for (size_t b = 0; b < Bs; b++) {
            fprintf(stderr,"Start C=A*B (optimized, column major, block size=%zu).\n",blockSize[b]);
            times3[b][m] = benchmark_AB(A, B_col, N, 3, blockSize[b], Ns[m]);
        }

        fprintf(stderr, "==================================================\n");
    }

    FILE* fp = nullptr;
    fp = fopen("matrix_matrix_times.txt", "w");
    // write header to the file
    std::string header = " N   unopt ";
    for (size_t b = 0; b < Bs; b++)
        header = header + "  br_" + std::to_string(blockSize[b]);
    for (size_t b = 0; b < Bs; b++)
        header = header + "  bc_" + std::to_string(blockSize[b]);
    header = header + "  lapack ";
    header = header + "\n";
    fprintf(fp, "%s", header.c_str());

    for (size_t m = 0; m < M; m++) {
        fprintf(fp, "%zu %lf", matrixSize[m], times1[m]);
        for (size_t b = 0; b < Bs; b++)
            fprintf(fp, " %lf ", times2[b][m]);
        for (size_t b = 0; b < Bs; b++)
            fprintf(fp, " %lf ", times3[b][m]);
        fprintf(fp, "%lf", times4[m]);
        fprintf(fp, "\n");
    }
    fclose(fp);

    //free allocated memory
    free(A);
    free(B_row);
    free(B_col);

    return 0;
}
