CC=g++
CFLAGS=-O3 -Wall -Wextra -Wpedantic

all: basic basic2 basic3

basic.o: basic.cpp
	$(CC) -c -o basic.o basic.cpp $(CFLAGS)

basic2.o: basic2.cpp
	$(CC) -c -o basic2.o basic2.cpp $(CFLAGS)

basic3.o: basic3.cpp
	$(CC) -c -o basic3.o basic3.cpp $(CFLAGS)

basic: basic.o
	$(CC) -o basic basic.o

basic2: basic2.o
	$(CC) -o basic2 basic2.o

basic3: basic3.o
	$(CC) -o basic3 basic3.o

clean:
	rm -f *.o *~ basic basic2 basic3

.PHONY: all
.PHONY: clean
