CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -Wpedantic
EXTRA =

LIBPATH=$(MKLROOT)/lib/intel64
MKL_SERIAL_LINK=-Wl,--start-group  $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm

targets = matrix_matrix

all: $(targets)

% : %.cpp
	$(CXX) $< -std=c++11  $(CXXFLAGS) $(EXTRA) $(MKL_SERIAL_LINK) -o $@

clean:
	rm -f *.o *~ $(targets) *.txt .tags

.PHONY: all
.PHONY: clean
