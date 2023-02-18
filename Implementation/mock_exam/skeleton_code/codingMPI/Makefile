MPICXX = mpicxx
CXXFLAGS = -O2 -g

all: diffusion

diffusion: diffusion.cpp
	$(MPICXX) $< -std=c++11 $(CXXFLAGS) -o $@

run1: diffusion
	mpirun -n 1 ./diffusion 1.0 2.0 256

runp: diffusion
	mpirun -n 2 ./diffusion 1.0 2.0 256

plot: plots.py
	python plots.py

clean:
	rm -r -f diffusion diagnostics.dat diffusion.dSYM *.00* *png

.PHONY: all clean
.SUFFIXES:
