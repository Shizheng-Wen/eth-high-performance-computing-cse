CC=g++
CFLAGS=-O3 -Wall -Wextra -Wpedantic
OBJ=factorial.o
DEPS=factorial.hpp

all: factorial

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

factorial: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *~ factorial

.PHONY: all
.PHONY: clean
