PREFIX ?=

CC	= $(PREFIX)gcc
CPP	= $(PREFIX)g++
AS	= $(CC)
LD	= $(PREFIX)ld
AR	= $(PREFIX)ar

ROOT = ./../CPP-Servo-Hat/bcm2835-1.68/rpidmx512

LIB := -L$(ROOT)/lib-pca9685/lib_linux
LDLIBS := -lpca9685 
LIBDEP := $(ROOT)/lib-pca9685/lib_linux/libpca9685.a

INCLUDES := -I$(ROOT)/lib-pca9685/include

COPS := -Wall -Werror -O3 -fno-rtti -std=c++11 -DNDEBUG

BCM2835 = $(ROOT)/lib-bcm2835_raspbian

ifneq "$(wildcard $(BCM2835) )" ""
	LIB += -L$(BCM2835)/lib_linux
	LDLIBS += -lbcm2835_raspbian
	INCLUDES += -I$(BCM2835)/include
else
	LDLIBS += -lbcm2835
endif

all : axis

clean :
	rm -f *.o
	rm -f *.lst
	rm -f axis
	cd $(ROOT)/lib-pca9685 && make -f Makefile.Linux clean
	
$(ROOT)/lib-pca9685/lib_linux/libpca9685.a :
	cd $(ROOT)/lib-pca9685 && make -f Makefile.Linux "DEF=-DRASPPI"

axis : Makefile axis.cpp $(ROOT)/lib-pca9685/lib_linux/libpca9685.a
	$(CPP) axis.cpp $(INCLUDES) $(COPS) -o axis $(LIB) $(LDLIBS)
	$(PREFIX)objdump -D axis | $(PREFIX)c++filt > axis.lst
	