all: runplot

# TODO 1: add OpenMP flags
main: main.cpp Makefile
	g++-12 -O3 -fopenmp -std=c++11 main.cpp -o main

run: main
	./main 100000 100

plot:
	./plot

runplot: run
	./plot

clean:
	rm -rf main out hist.pdf hist_0.dat hist_1.dat

.PHONY: all clean run plot runplot
