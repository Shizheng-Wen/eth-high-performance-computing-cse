MPICXX = mpic++
CXXFLAGS = -O3 -g

all: diffusion

diffusion: diffusion.cpp
	$(MPICXX) $< -std=c++11 $(CXXFLAGS) -o $@

run: diffusion
	mkdir -p out
	mpirun -n 1 ./diffusion 1.0 2.0 100

clean:
	rm -rf diffusion diagnostics.* *.png *.txt out*

.PHONY: all clean
.SUFFIXES:
