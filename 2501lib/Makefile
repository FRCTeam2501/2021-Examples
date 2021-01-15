LIB_ROOT = /home/pi
LIB_DIR = lib2501

CXX = g++
INCFLAGS = -I$(LIB_ROOT)/$(LIB_DIR)/rpidmx512/lib-pca9685/include
CXXFLAGS = -Wall -Werror -O3 -fno-rtti -std=c++11 -DNDEBUG
LFLAGS = -L$(LIB_ROOT)/$(LIB_DIR)/rpidmx512/lib-pca9685/lib_linux -lpca9685 -lbcm2835

OBJS = $(patsubst %.cpp, %.out, $(wildcard *.cpp))

all: $(OBJS)
	ar rcs lib2501.a $(OBJS)

%.out: %.cpp Makefile
	$(CXX) -c $< $(INCFLAGS) $(CXXFLAGS) -o $@ $(LFLAGS)

clean:
	rm -f *.out
	rm -f *.a