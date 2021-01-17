CXX = g++
INCFLAGS = 
CXXFLAGS = -Wall -Werror -O3 -fno-rtti -std=c++11 -DNDEBUG
LFLAGS = -l2501 -lpca9685 -lbcm2835

OBJS = $(patsubst %.cpp, %.out, $(wildcard *.cpp))

all: $(OBJS)
	$(CXX) -o main.exe $(OBJS) $(INCFLAGS) $(CXXFLAGS) $(LFLAGS)

%.out: %.cpp Makefile
	$(CXX) -c $< -o $@ $(INCFLAGS) $(CXXFLAGS) $(LFLAGS)

clean:
	rm -f *.out
	rm -f *.exe