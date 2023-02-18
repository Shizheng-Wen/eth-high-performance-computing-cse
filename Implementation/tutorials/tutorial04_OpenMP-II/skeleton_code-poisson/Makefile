CFLAGS = -O3 -g -Wall -std=c++11 -fopenmp

all: poisson

poisson: poisson.cpp
	g++ $(CFLAGS) -o poisson poisson.cpp
clean:
	rm -f *.o *~ poisson
