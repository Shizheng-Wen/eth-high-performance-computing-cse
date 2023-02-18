# for more info on Makefiles, see e. g. http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
CC=g++
CFLAGS=-O3 -Wall -Wextra -Wpedantic

all: main

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o factorial.o
	$(CC) -I. -o main main.o factorial.o

clean:
	rm -f *.o *~ main

.PHONY: all
.PHONY: clean
