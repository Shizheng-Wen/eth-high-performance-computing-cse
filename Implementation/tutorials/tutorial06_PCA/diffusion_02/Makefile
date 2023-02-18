CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -pedantic -std=c++14 -fopenmp

all: diffusion diffusion_naive

diffusion: diffusion.cpp
	$(CXX) $< $(CXXFLAGS) -o $@

diffusion_naive: diffusion_naive.cpp
	$(CXX) $< $(CXXFLAGS) -o $@

run: diffusion
	./diffusion 100 1

jobs: diffusion
	./run.sh

stats: diffusion
	./stats.sh


clean:
	rm -rf diffusion diffusion_naive lsf*

.PHONY: all clean
.SUFFIXES:
