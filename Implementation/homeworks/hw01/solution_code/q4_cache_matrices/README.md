```
Q4a. Hadamard product. This code can be ran on your own pc. But, you can also run it on euler if wanted.
We did not prepare a makefile for this code, because it is so small it doesn't need one. You can directly compile it
inside the terminal in this way:

# g++ -o hadamard -std=c++11 <any flags you want> hadamard.cpp

where <any flags you want> can be (-O0, -ffast-math, -O3, ...)
To run this script, we prepared a bash script called "run_hadamard.sh".
Modify it with however you want with any size of matrix you want inside the for-loop.
Run the script with "./run_hadamard.sh" or "source run_hadamard.sh"

###########################################################

Q4b-c. Matrix-Matric multiplication:

This code is highly recommended to run on euler, due to the Lapack library being easily accessable on euler via loading with:

module load mkl
module load python/3.7.1

You can try to run it on your own pc but you have to install the mkl library on your own.

To compile run:
make matrix_matrix

When trying to run submit the code with either one of these commands:
# bsub -n 1 -R "select[model==XeonGold_6150]" -W 01:00 -Is bash
or
# bsub -n 1 -R "select[model==XeonGold_6150]" -W 01:00 ./matrix_matrix

The first will put you direclty inside the working node for 1 hour. This is usefull for directly debugging.
But it will take some time to get direct access to the node.
Inside the node, compile and type "./matrix_matrix" to run it.
The second method will directly put the code on a queue of the CPU model Xeon Gold 6150. It is very important, that
we run the code on an intel processor, otherwise it will not run lapack functions. 
# The executable will provide you with the average timing for Ns=10 iterations of the matrix-matrix multiplication for:
# 3 matrix sizes: 512, 1024, 2048
# row and column major format 
# non-optimized algorithm and blocking algorithm for 9 block size choices.
# the executable will also create the txt file matrix_matrix_times.txt, which can be used to plot the timings with python:


To plot use this:
python3 plot_matrix_matrix.py 

############################################################

```
