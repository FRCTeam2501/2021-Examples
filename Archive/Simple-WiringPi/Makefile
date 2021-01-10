CXX = g++
CXXFLAGS = -Wall -lwiringPi -lpthread

all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp Makefile
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.out
