CC=g++

LIBPATH=$(MKLROOT)/lib/intel64
MKL_SERIAL_LINK=-Wl,--start-group  $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm
MKL_PARA_LINK=-Wl,--start-group  $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_gnu_thread.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm

CPPFLAGS= -std=c++11 -fopenmp
CPPFLAGS+= -fstrict-aliasing -mtune=native  -march=native
CPPFLAGS+= -fmerge-all-constants -fassociative-math
CPPFLAGS += -O3 -ffast-math
//CPPFLAGS+= -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wpedantic
//CPPFLAGS+= -Wmissing-declarations -Wredundant-decls -Wshadow -Wwrite-strings
//CPPFLAGS+= -Woverloaded-virtual -Wno-unused-parameter

LIBS= -lstdc++ -lm
LIBS+= -lpthread -lz -ldl

main: main.o
	$(CC) $(CPPFLAGS) $(LIBS) $(MKL_PARA_LINK) main.o $(MKL_PARA_LINK) -o $@

all: main
.DEFAULT_GOAL := all

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.dSYM *.s *.d main

