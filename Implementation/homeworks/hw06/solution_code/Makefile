CXX = mpic++
CXXFLAGS = -O3 -Wall -Wextra -std=c++11
EXTRA ?=

all: main

main: main.cpp Makefile *.h
	$(CXX) $(CXXFLAGS) $(EXTRA) main.cpp -o main

run: 
	./run

plot:
	./plotall

runplot: run plot

clean:
	rm -vf main u_*.dat u_*.png

.PHONY: all clean run plot runplot
