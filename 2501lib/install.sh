#!/bin/bash

# Defining variables
LIB_ROOT=~
LIB_DIR=2501lib


# Move to correct directory
echo -e '\033[0;31m***** MAKING DIRECTORY *****\033[0m'
cd ${LIB_ROOT}
mkdir ${LIB_DIR}
cd ${LIB_DIR}


# Download and extract BCM2835 library
echo -e '\033[0;31m***** INSTALLING BCM2835 *****\033[0m'
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.68.tar.gz
tar zxvf bcm2835-1.68.tar.gz
cd bcm2835-1.68
./configure
make
sudo make check
sudo make install


# Download and extract RPIDMX512 library
echo -e '\033[0;31m***** INSTALLING RPIDMX512 *****\033[0m'
cd ${LIB_ROOT}/${LIB_DIR}
git clone https://github.com/TylerSeiford/rpidmx512.git
cd rpidmx512/lib-pca9685/
make -f Makefile.Linux "DEF=-DRASPPI"