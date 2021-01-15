LIB_ROOT = /home/pi
LIB_DIR = lib2501

CXX = g++
INCFLAGS = 
CXXFLAGS = -Wall -Werror -O3 -fno-rtti -std=c++11 -DNDEBUG
LFLAGS = -lpca9685 -lbcm2835

OBJS = $(patsubst %.cpp, %.out, $(wildcard *.cpp))

all: $(OBJS)
	ar rcs lib2501.a $(OBJS)

%.out: %.cpp Makefile
	$(CXX) -c $< $(INCFLAGS) $(CXXFLAGS) -o $@ $(LFLAGS)

clean:
	rm -f *.out
	rm -f *.a