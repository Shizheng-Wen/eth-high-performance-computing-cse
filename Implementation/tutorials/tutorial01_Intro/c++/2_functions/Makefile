# for more info on Makefiles, see e. g. http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
CC=g++
CFLAGS=-O3 -Wall -Wextra -Wpedantic

all: function

*.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

function: function.o
	$(CC) -o function function.o

clean:
	rm -f *.o *~ function

.PHONY: all
.PHONY: clean
