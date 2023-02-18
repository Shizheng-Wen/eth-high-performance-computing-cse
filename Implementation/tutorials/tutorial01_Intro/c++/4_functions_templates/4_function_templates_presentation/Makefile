# for more info on Makefiles, see e. g. http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
CC=g++
CFLAGS=-O3 -Wall -Wextra -Wpedantic

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o
	$(CC) -I. -o main main.o

all: main

clean:
	rm -f *.o *~ main

.PHONY: all
.PHONY: clean
