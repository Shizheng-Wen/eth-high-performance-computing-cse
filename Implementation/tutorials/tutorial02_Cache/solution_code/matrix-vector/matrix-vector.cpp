#include<chrono>
#include<stdio.h>
#include<iostream>
#include<cmath>

void mv_row_wise(double* A, double* b, double* c, int N){
    for (int i = 0; i < N; i++)
        c[i] = 0;

    // TODO: Matrix-Vector multiplication row-wise
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            c[i] += A[i * N + j] * b[j];
}

void mv_col_wise(double* A, double* b, double* c, int N){
    for (int i = 0; i < N; i++)
        c[i] = 0;

    // TODO: Matrix-Vector multiplication col-wise
    for (int j = 0; j < N; j++)
        for (int i = 0; i < N; i++)
            c[i] += A[i * N + j] * b[j];
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Error: give me matrix size\n");
        return 1;
    }

    const int N = atoi(argv[1]);

    printf("\n================================\n");
    printf("N = %u\n", N);


    //TODO: Randomly initialize the matrix A and vector b
    double* A = (double*) malloc(N*N*sizeof(double));
    double* b = (double*) malloc(N*sizeof(double));
    double* c = (double*) malloc(N*sizeof(double));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            A[i*N + j] = 3.14156*i/(j+1);
        }
        b[i] = A[i*N + i]*0.5;
    }

    const int reps = 1e3;

    printf("Warm-up runs...\n");
    auto time_start = std::chrono::system_clock::now();
    for(int i = 0; i < 1e2; i++){
        mv_row_wise(A, b, c, N);
    }
    auto time_warm_up = std::chrono::duration<double>(std::chrono::system_clock::now() - time_start).count()/reps;

    printf("Warm-up run took %9.4fs\n", time_warm_up);

    printf("start running c = A*b row-wise benchmark...\n");
    // TODO: benchmark your mv_row_wise function using std::chrono and with reps=1e3. 
    // Save the time in the variable "time_row". Measure in seconds!

    time_start = std::chrono::system_clock::now();
    for(int i = 0; i < reps; i++){
        mv_row_wise(A, b, c, N);
    }
    auto time_row = std::chrono::duration<double>(std::chrono::system_clock::now() - time_start).count()/reps;

    printf("Matrix-Vector multiplication row-wise time %9.4fs\n", time_row);

    double* c_temp = (double*) malloc(N*sizeof(double));
    for(int i = 0; i < N; i++){
        c_temp[i] = c[i];
    }


    printf("start running  c = A*b col-wise benchmark...\n");
    // TODO: benchmark your mv_col_wise function using std::chrono and with reps=1e3.
    // Save the time in the variable "time_col". Measure in seconds!

    time_start = std::chrono::system_clock::now();
    for(int i = 0; i < reps; i++){
        mv_col_wise(A, b, c, N);
    }
    auto time_col = std::chrono::duration<double>(std::chrono::system_clock::now() - time_start).count()/reps;

    printf("Matrix-Vector multiplication col-wise time %9.4fs\n", time_col);

    double error = 0.0;
    for(int i = 0; i < N; i++){
        error += std::abs(c_temp[i] - c[i]);
    }

    if(error > 1e-10){
        printf("======================================================\n");
        printf("ERROR: row-wise and col-wise are not similar! error = %.40f\n", error);
        printf("======================================================\n");
        return 1;
    }

    printf("T(col-wise)/T(row-wise) = %9.4f\n", time_col/time_row);

    //free allocated memory here:
    free(A);
    free(b);
    free(c);

    return 0;
}