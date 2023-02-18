CXX=mpicxx
CXXFLAGS=-O3 -Wall -std=c++11

all: monte-carlo quadratic poisson

monte-carlo: monte-carlo.cpp
	$(CXX) $(CXXFLAGS) monte-carlo.cpp -o monte-carlo
quadratic: quadratic.cpp
	$(CXX) $(CXXFLAGS) quadratic.cpp -o quadratic
poisson: poisson.cpp
	$(CXX) $(CXXFLAGS) poisson.cpp -o poisson
clean:
	rm -rf monte-carlo quadratic poisson

.PHONY: all clean
