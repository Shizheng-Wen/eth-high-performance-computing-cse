# for more info on Makefiles, see e. g. http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
CC=g++
CFLAGS=-O3 -Wall -Wextra -Wpedantic -std=c++11

all: basic_class_example

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

basic_class_example: basic_class_example.o simpleclass.o
	$(CC) -I. -o basic_class_example basic_class_example.o simpleclass.o

clean:
	rm -f *.o *~ basic_class_example

.PHONY: all
.PHONY: clean
