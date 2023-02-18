# Makefile for HPCSE I (2018HS) Exam, 11.01.2019
# Prof. P. Koumoutsakos
# Coding 1: Julia Set

CXXFLAGS = -O3 -Wall -Wextra -std=c++11 -march=native -fopenmp

.PHONY: clean run plot

julia_set: julia_set.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f julia_set image.txt histogram.txt image.png histogram.png

run: julia_set
	OMP_NUM_THREADS=1 ./julia_set
	OMP_NUM_THREADS=2 ./julia_set
	OMP_NUM_THREADS=4 ./julia_set

plot:
	python3 plot.py
