CC=g++
CFLAGS=-O3 -Wall -Wextra -Wpedantic

all: factorial

factorial.o: factorial.cpp
	$(CC) -c -o factorial.o factorial.cpp $(CFLAGS)

factorial: factorial.o
	$(CC) -o factorial factorial.o

clean:
	rm -f *.o *~ factorial

.PHONY: all
.PHONY: clean