config ?= debug

ifeq ($(shell uname -s), Darwin)
CC=gcc-12
else
CC=gcc
endif

LIBPATH=$(MKLROOT)/lib/intel64
MKL_SERIAL_LINK=-Wl,--start-group  $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm
MKL_PARA_LINK=-Wl,--start-group  $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_gnu_thread.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm

CPPFLAGS= -std=c++11 -fopenmp
CPPFLAGS+= -fstrict-aliasing -mtune=native  -march=native
CPPFLAGS+= -fmerge-all-constants -fassociative-math

LIBS= -lstdc++ -lm
LIBS+= -lpthread -lz -ldl

ifeq "$(config)" "debug"
CPPFLAGS+= -g -O0
endif

ifeq "$(config)" "prod"
CPPFLAGS += -DNDEBUG -O3 -ffast-math
endif


CPPFLAGS+= -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wpedantic
CPPFLAGS+= -Wmissing-declarations -Wredundant-decls -Wshadow -Wwrite-strings
CPPFLAGS+= -Woverloaded-virtual -Wno-unused-parameter


exec_pca: main_pca.o
	$(CC) $(CPPFLAGS) $(LIBS) $(MKL_PARA_LINK) main_pca.o $(MKL_PARA_LINK) -o $@


all: exec_pca
.DEFAULT_GOAL := all

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.dSYM *.s *.d exec_*



