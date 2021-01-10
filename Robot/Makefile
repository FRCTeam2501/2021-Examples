LIB_ROOT = /home/pi
LIB_DIR = 2501lib

CXX = g++
INCFLAGS = -I$(LIB_ROOT)/$(LIB_DIR)/rpidmx512/lib-pca9685/include
CXXFLAGS = -Wall -Werror -O3 -fno-rtti -std=c++11 -DNDEBUG
LFLAGS = -L$(LIB_ROOT)/$(LIB_DIR)/rpidmx512/lib-pca9685/lib_linux -lpca9685 -lbcm2835

all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp Makefile
	$(CXX) $< $(INCFLAGS) $(CXXFLAGS) -o $@ $(LFLAGS)

clean:
	rm -f *.out