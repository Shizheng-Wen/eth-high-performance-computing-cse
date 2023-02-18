CFLAGS = -O3 -g -Wall -std=c++11 -fopenmp -lopenblas
CFLAGS += -I$(BLASROOT)/include
CFLAGS += -L$(BLASROOT)/lib

all: gemm

gemm: gemm.cpp
	g++ $(CFLAGS) -o gemm gemm.cpp
clean:
	rm -f *.o *~ gemm
