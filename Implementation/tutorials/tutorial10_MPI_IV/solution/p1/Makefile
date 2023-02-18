CXX=mpicxx
CXXFLAGS=-O3 -Wall -std=c++11

all: main

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
clean:
	rm -rf *.o *.xmf *txt main

main: main.o auxiliary.o wave.o
	$(CXX) $(CXXFLAGS) -I. -o main $^

run:
	make clean ; make ; mpirun -n 8 ./main 100 2 1.0	
run1:
	make clean ; make ; mpirun -n 1 ./main 100 1 1.0	

.PHONY: all clean run run1
