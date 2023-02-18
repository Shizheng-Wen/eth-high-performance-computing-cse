CXX ?= g++
CXXFLAGS = -O3 -Wall -Wextra -pedantic -std=c++14 -fopenmp

all: diffusion

diffusion: diffusion.cpp
	$(CXX) $< $(CXXFLAGS) -o $@

run: diffusion
	./diffusion 100 1

stat: diffusion
	export OMP_NUM_THREADS=12; ./varym 100 0

statjob: diffusion
	export OMP_NUM_THREADS=12; bsub -n 12 -W 00:30 ./varym 100 0

plotstat: out
	./plotstat

clean:
	rm -rf diffusion diagnostics.dat out lsf*

.PHONY: all clean
.SUFFIXES:
