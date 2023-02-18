CXX = g++
MPICXX = mpicxx

CXXFLAGS=-O3 -Wall -std=c++11

TARGETS = serial mpi mpi_non_blocking

all: $(TARGETS)

serial: serial.cpp
	$(CXX) $< -std=c++11 $(CXXFLAGS) -o $@

mpi: mpi.cpp
	$(MPICXX) $< -std=c++11 $(CXXFLAGS) -o $@

mpi_non_blocking: mpi_non_blocking.cpp
	$(MPICXX) $< -std=c++11 $(CXXFLAGS) -o $@

clean:
	rm -f $(TARGETS)

.PHONY: all clean
