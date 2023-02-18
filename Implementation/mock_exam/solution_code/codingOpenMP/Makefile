CFLAGS = -O3 -g -Wall -std=c++11 -fopenmp

all: diffusion

diffusion: diffusion_adi.cpp
	g++ $(CFLAGS) -o diffusion diffusion_adi.cpp
clean:
	rm -f *.o diffusion 
