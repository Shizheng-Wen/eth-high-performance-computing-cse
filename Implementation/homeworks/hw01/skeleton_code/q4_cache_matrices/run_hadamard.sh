#!/bin/sh

#BEFORE running this script, compile your hadamard.cpp script like this: 
#
#g++ -o hadamard -std=c++11 <add flags here> hadamard.cpp 
#
#run this script with ./run_hadamard.sh

#i being the matrix size in one dimension. So i = 300 -> matrix with dim(300,300)
for i in 100 200 400 800    
do
    ./hadamard $i
done
