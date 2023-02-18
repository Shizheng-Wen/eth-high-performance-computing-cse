CXX=g++
CXXFLAGS=-O3 -Wall -Wextra -std=c++11

.PHONY: all clean run plot submit

all: cache_size

clean:
	rm -f cache_size results.txt results.png lsf.*

run:
	-lscpu
	-grep . /sys/devices/system/cpu/cpu0/cache/index*/*
	./cache_size | tee results.txt
	# Results stored into 'results.txt', run 'make plot'.

plot:
	./plot.sh
	# Plot stored into 'results.png'.

submit:
	bsub -R "select[model==XeonGold_6150]" -R fullnode -n 36 -W 00:10 make run

cache_size: cache_size.cpp Makefile
	$(CXX) $(CXXFLAGS) cache_size.cpp -o cache_size
