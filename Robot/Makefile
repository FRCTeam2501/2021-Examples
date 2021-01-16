CXX = g++
INCFLAGS = 
CXXFLAGS = -Wall -Werror -O3 -fno-rtti -std=c++11 -DNDEBUG
LFLAGS = -l2501 -lbcm2835 -lpca9685

all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp Makefile
	$(CXX) $< $(INCFLAGS) $(CXXFLAGS) -o $@ $(LFLAGS)

clean:
	rm -f *.out