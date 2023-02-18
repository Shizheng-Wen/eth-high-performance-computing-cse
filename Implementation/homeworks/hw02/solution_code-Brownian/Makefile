CXX ?= g++

# TODO: add OpenMP flags
CXXFLAGS = -O3 -Wall -Wextra -std=c++11 -fopenmp


all: runplot

main: main.cpp Makefile
	$(CXX) $(CXXFLAGS) $(EXTRA) main.cpp -o main

run: main
	OMP_NUM_THREADS=4 ./main 10000 1000

run_seed: seed
	./seed

run_scaling: main scaling
	OMP_PROC_BIND=true ./scaling 10000000 10 > time1bind.dat
	OMP_PROC_BIND=false ./scaling 10000000 10 > time1nobind.dat
	OMP_PLACES=cores ./scaling 10000000 10 > time1cores.dat

plot:
	./plot

runplot: run
	./plot

clean:
	rm -rf main out hist.pdf

.PHONY: all clean run plot runplot
