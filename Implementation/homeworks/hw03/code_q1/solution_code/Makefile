CXX = g++-12
CXXFLAGS = -O3 -Wall -Wextra -pedantic -std=c++14 -fopenmp

all: diffusion

diffusion: diffusion.cpp
	$(CXX) $< $(CXXFLAGS) -o $@

run: diffusion
	./diffusion 1.0 2.0 100 0.5 1

stat: diffusion
	export OMP_NUM_THREADS=12; ./varym 1.0 2.0 100 0.5 0

statjob: diffusion
	export OMP_NUM_THREADS=12; bsub -n 12 -W 00:30 ./varym 1.0 2.0 100 0.5 0

statjobfull: diffusion
	export OMP_NUM_THREADS=12; bsub -n 24 -R fullnode -W 00:30 ./varym 1.0 2.0 100 0.5 0

plotstat: out
	./plotstat

clean:
	rm -rf diffusion diagnostics.dat out

.PHONY: all clean
.SUFFIXES:
