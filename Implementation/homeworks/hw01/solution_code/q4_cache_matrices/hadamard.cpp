#include<chrono>
#include<stdio.h>
#include<iostream>
#include<cmath>

void hadamard_row_wise(double* A, double* B, double* C, int N){
    for (size_t i = 0; i < N*N; i++)
        C[i] = 0;

    // TODO: Hadamard product row-wise
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
                C[i * N + j] = A[i * N + j] * B[i * N + j];
}

void hadamard_col_wise(double* A, double* B, double* C, int N){
    for (size_t i = 0; i < N*N; i++)
        C[i] = 0;

    // TODO: Hadamard product col-wise
    for (size_t j = 0; j < N; j++)
        for (size_t i = 0; i < N; i++)
                C[i * N + j] = A[i * N + j] * B[i * N + j];
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Error: give me matrix size\n");
        return 1;
    }

    const int N = atoi(argv[1]);

    printf("\n================================\n");
    printf("N = %u\n", N);


    //TODO: Randomly initialize the matrices A,B
    double* A = (double*) calloc(N*N, sizeof(double));
    double* B = (double*) calloc(N*N, sizeof(double));
    double* C = (double*) calloc(N*N, sizeof(double)); //calloc = malloc but set to zeros
    for(int i = 0; i < N*N; i++){
        A[i] = 3.14156*i/(i+1);
        B[i] = A[i]*0.5;
    }

    const int reps = 1e3;

    printf("start running Hadamard C = A°B row-wise benchmark...\n");
    // TODO: benchmark your hadamard_row_wise function using std::chrono and with reps=1e3. 
    // Save the time in the variable "time_row". Measure in seconds!

    auto time_start = std::chrono::system_clock::now();
    for(int i = 0; i < reps; i++){
        hadamard_row_wise(A, B, C, N);
    }
    auto time_row = std::chrono::duration<double>(std::chrono::system_clock::now() - time_start).count()/reps;

    printf("Hadamard row-wise time %9.4fs\n", time_row);

    double* C_temp = (double*) calloc(N*N, sizeof(double));
    for(int i = 0; i < N*N; i++){
        C_temp[i] = C[i];
    }


    printf("start running Hadamard C = A°B col-wise benchmark...\n");
    // TODO: benchmark your hadamard_col_wise function using std::chrono and with reps=1e3.
    // Save the time in the variable "time_col". Measure in seconds!

    time_start = std::chrono::system_clock::now();
    for(int i = 0; i < reps; i++){
        hadamard_col_wise(A, B, C, N);
    }
    auto time_col = std::chrono::duration<double>(std::chrono::system_clock::now() - time_start).count()/reps;

    printf("Hadamard row-wise time %9.4fs\n", time_col);

    double error = 0.0;
    for(int i = 0; i < N*N; i++){
        error += std::abs(C_temp[i] - C[i]);
    }

    if(error > 1e-10){
        printf("======================================================\n");
        printf("ERROR: row-wise and col-wise are not similar! error = %.40f\n", error);
        printf("======================================================\n");
        return 1;
    }

    printf("T(col-wise)/T(row-wise) = %9.4f\n", time_col/time_row);

    //free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}