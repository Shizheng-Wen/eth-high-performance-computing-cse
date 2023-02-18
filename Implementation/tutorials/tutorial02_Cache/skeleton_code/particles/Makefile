CXX = g++
CXXFLAGS = -Ofast -std=c++11 -Wall -Wextra -funroll-loops

targets = particles

all: $(targets)

% : %.cpp helpers.hpp
	mkdir states -p
	mkdir figures -p
	$(CXX) $< $(CXXFLAGS) -o $@

mov:
	ffmpeg -framerate 0.1 -pattern_type glob -i './figures/*.png' -c:v libx264 -r 30 -pix_fmt yuv420p simulation.mp4

clean:
	rm -f *.o *~ $(targets) *.txt states/*.txt figures/*.png

.PHONY: all
.PHONY: clean
