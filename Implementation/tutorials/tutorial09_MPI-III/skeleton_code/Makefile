CXX = mpic++
CXXFLAGS = -std=c++11 -Wall -Wextra

main: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

all: main run plot

run:
	mpirun -n 2 ./main

plot:
	sh ./plot

clean:
	rm -f main init.dat final.dat image.png

.PHONY: all run plot clean


