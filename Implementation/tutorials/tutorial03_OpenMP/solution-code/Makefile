CXX ?= g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++11 -fopenmp

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main
clean:
	rm -rf main out results.png

.PHONY: all clean
