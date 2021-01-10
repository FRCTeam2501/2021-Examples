#!/bin/bash

# Defining variables
LIB_ROOT=~
LIB_DIR=2501lib



# Download and extract BCM2835 library
echo -e '\033[1;36m***** INSTALLING BCM2835 *****\033[0m'
cd /tmp
echo -e '\033[1;36m - Getting library\033[0m'
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.68.tar.gz
echo -e '\033[1;36m - Extracting library\033[0m'
tar zxvf bcm2835-1.68.tar.gz
cd bcm2835-1.68
echo -e '\033[1;36m - Configuring library\033[0m'
./configure
echo -e '\033[1;36m - Building library\033[0m'
make
echo -e '\033[1;36m - Verifying library\033[0m'
sudo make check
echo -e '\033[1;36m - Installing library\033[0m'
sudo make install


# Download and extract RPIDMX512 library
echo -e '\033[1;36m***** INSTALLING RPIDMX512 *****\033[0m'
cd ${LIB_ROOT}
mkdir ${LIB_DIR}
cd ${LIB_DIR}
echo -e '\033[1;36m - Getting library\033[0m'
git clone https://github.com/vanvught/rpidmx512.git
cd rpidmx512/lib-pca9685/
echo -e '\033[1;36m - Building library\033[0m'
make -f Makefile.Linux "DEF=-DRASPPI"