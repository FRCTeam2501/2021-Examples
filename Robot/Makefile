LIB_ROOT = /home/pi
LIB_DIR = lib2501

CXX = g++
INCFLAGS = -I$(LIB_ROOT)/$(LIB_DIR)/rpidmx512/lib-pca9685/include -I../2501lib
CXXFLAGS = -Wall -Werror -O3 -fno-rtti -std=c++11 -DNDEBUG
LFLAGS = -L$(LIB_ROOT)/$(LIB_DIR)/rpidmx512/lib-pca9685/lib_linux -lpca9685 -lbcm2835 -L../2501lib -l2501

all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp Makefile
	$(CXX) $< $(INCFLAGS) $(CXXFLAGS) -o $@ $(LFLAGS)

clean:
	rm -f *.out