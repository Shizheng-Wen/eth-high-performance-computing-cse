all: assertions printdebug matrixmult Burgers1D debug sanitizer profiler

assertions: assertions.cpp
	g++ -g -Wall -o $@ $<

printdebug: printdebug.cpp
	g++ -g -Wall -o $@ $<

matrixmult: matrixmult.cpp
	g++ -g -Wall -o $@ $<

Burgers1D: Burgers1D.cpp
	g++ -g -DNDEBUG -Wall -o $@ $<

debug: Burgers1D.cpp
	g++ -g -Wall -o $@ $<

sanitizer: Burgers1D.cpp
	g++ -g -DNDEBUG -Wall -fsanitize=address -o $@ $<

profiler: Burgers1D.cpp
	g++ -g -DNDEBUG -pg -Wall -o $@ $<

clean:
	rm -r -f assertions printdebug matrixmult Burgers1D debug sanitizer profiler example.txt *.png *.out *.dSYM
