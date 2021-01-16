CXX = g++
INCFLAGS = 
CXXFLAGS = -Wall -Werror -O3 -fno-rtti -std=c++11 -DNDEBUG
LFLAGS = -lbcm2835 -lpca9685

OBJS = $(patsubst %.cpp, %.out, $(wildcard *.cpp))

all: $(OBJS)
	ar rcs lib2501.a $(OBJS)

%.out: %.cpp Makefile
	$(CXX) -c $< -o $@ $(INCFLAGS) $(CXXFLAGS) $(LFLAGS)

clean:
	rm -f *.out
	rm -f *.a

install:
	mkdir -p /usr/local/include/2501
	cp *.h /usr/local/include/2501
	cp lib2501.a /usr/local/lib