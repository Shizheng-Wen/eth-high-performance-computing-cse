CC=g++
CFLAGS=-O3 -Wall -Wextra -Wpedantic -DNDEBUG

all: matrix-vector

matrix-vector.o: matrix-vector.cpp
	$(CC) -c -o matrix-vector.o matrix-vector.cpp $(CFLAGS)

matrix-vector: matrix-vector.o
	$(CC) -o matrix-vector matrix-vector.o

clean:
	rm -f *.o *~ matrix-vector

.PHONY: all
.PHONY: clean
