#ifndef UTILS_H_UY1M7JFH
#define UTILS_H_UY1M7JFH

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <cassert> /* assert */

namespace utils{

    double* loadDataset(const std::string & data_path, const int N, const int D)
    {
        // Input file stream object to read data
        std::ifstream inputFile;
        inputFile.open(data_path);

        const std::string delimiter = ",";
        std::string line;
        std::string strnum;

        size_t pos = 0;
        std::string token;

        double *data = new double[N * D];

        int stillToLoad = 0;
        int n = 0;
        // parse line by line
        while (std::getline(inputFile, line))
        {
            // Input dimension
            int d = 0;
            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                double number = std::stod(token);
                data[n*D + d] = number;
                d++;
                line.erase(0, pos + delimiter.length());
                stillToLoad = N-n;
                if(stillToLoad<0){
                    break;
                }
            }
            if(stillToLoad<=0){
                break;
            }
            // Last element:
            token = line;
            double number = std::stod(token);
            data[n*D + d] = number;
            d++;
            n++;
        }
        std::cout << "Number of datapoints loaded: " << n << '\n';
        return data;
    }

    int writeRowMajorMatrixToFile(const std::string &data_path,
                                  const double *M,
                                  const int rows,
                                  const int cols)
    {
        // Row major encoding M(i,j)=M[i*cols+j]
        std::ofstream oFile;
        oFile.open(data_path);
        for(int i=0; i<rows;++i){
            for(int j=0; j<cols;++j){
                oFile << M[i*cols+j];
                if(j<cols-1){
                    oFile << ",";
                }
            }
            oFile << "\n";
        }
        oFile.close();
        return 0;
    }

    void reverseArray(double* data, const int D)
    {
        int DD = (int)std::ceil((double)D / 2.0);
        for (int j = 0; j < DD; ++j) {
            double temp = data[j];
            data[j] = data[D-j-1];
            data[D-j-1] = temp;
        }
    }

    void transposeData(double* data_T, const double* const data, const int N, const int D)
    {
        // Data are given in the form data(n,d)=data[n*D+d]
        // This function transposes the data to data(n,d)=data_T[d*N+n]
        // for the purpose of more efficient memory layout
        // (e.g. calculation of the mean)
        // TODO 
    }

    void computeMean(double* mean, const double* const data_T, const int N, const int D)
    {
        // Calculation of the mean (over samples) of the dataset
        // data(n,d)=data_T[d*N+n]
        // TODO
    }

    void computeStd(double* std, const double* const mean, const double* const data_T, const int N, const int D)
    {
        // Calculation of the mean (over samples) of the dataset
        // data(n,d)=data_T[d*N+n]
        // TODO
    }

    void centerDataColMajor(double* data_T, const double* const mean, const int N, const int D)
    {
        std::cout << "Centering data..." << std::endl;
        // COL MAJOR IMPLEMENTATION
        // data(n,d)=data_T[d*N + n]
        // TODO
    }

    void constructCovariance(double* C, const double* const data_T, const int N, const int D)
    {
        // Construct the covariance matrix (DxD) of the data.
        // data(n,d)=data_T[d*N+n]
        // For the covariance follow the ROW major notation
        // C(j,k)=C[j*D+k]
        // TODO
    }

    void getEigenvectors(double* V, const double* const C, const int NC, const int D)
    {
        // Extracting the last rows from matrix C containig the PCA components (eigenvectors of the covariance matrix) that explain the highest variance.
        // Be carefull to extract them in order of descenting variance.
        // C(j,d)=C[j*D+d] # ROW MAJOR
        // V(k,d)=V[k*D+d] # ROW MAJOR
        // TODO

    }

    void reduceDimensionality(double* data_red, const double* const V, const double* const data_T, const int N, const int D, const int NC)
    {
        // data(n,d)=data_T[d*N+n] (transposed dataset)
        // V(k,d)=V[k*D+d]
        // data_red(n,k)=data_red[n*NC + k], K<<D
        for (int n = 0; n < N; ++n) // Iterate through all data
        {
            for (int k = 0; k < NC; ++k) // Iterate through all components
            {
                double sum = 0.0;
                for (int d = 0; d < D; ++d) // Iterate through the dimensions
                {
                    sum += V[k*D + d] * data_T[d*N + n];
                }
                data_red[n*NC + k] = sum;
            }
        }
    }

    void reconstructDatasetRowMajor(double* data_rec, const double* const V, const double* const data_red, const int N, const int D, const int NC)
    {
        // ROW MAJOR
        // V(c,d)=V[d + c*D]
        // data_red(n,c)=data_red[c + n*NC], C<<D   # ROW MAJOR
        // data_rec(n,d)=data_rec[d + n*D]          # ROW MAJOR
        // TODO
    }

    void inverseCenterDatasetRowMajor(double* data_rec, const double* const mean, const int N, const int D)
    {
        // ROW MAJOR
        // data_rec(n,d)=data_rec[d + n*D]          # ROW MAJOR
        for (int n = 0; n < N; ++n) // Iterate through all data
        {
            for (int d = 0; d < D; ++d) // Iterate through all dimensions
            {
                data_rec[n*D + d] = data_rec[n*D + d] + mean[d];
            }
        }
    }

}

#endif /* UTILS_H_UY1M7JFH */
