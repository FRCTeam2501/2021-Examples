#!/bin/bash

# Defining variables
LIB_ROOT=~
LIB_DIR=2501lib


# Move to correct directory
echo '***** MAKING DIRECTORY *****'
cd ${LIB_ROOT}
mkdir ${LIB_DIR}
cd ${LIB_DIR}


# Download and extract BCM2835 library
echo '***** INSTALLING BCM2835 *****'
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.68.tar.gz
tar zxvf bcm2835-1.68.tar.gz
cd bcm2835-1.68
./configure
make
sudo make check
sudo make install


# Download and extract RPIDMX512 library
echo '***** INSTALLING RPIDMX512 *****'
cd ${LIB_ROOT}/${LIB_DIR}
git clone https://github.com/vanvught/rpidmx512.git
