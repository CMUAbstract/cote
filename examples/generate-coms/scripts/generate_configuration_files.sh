#!/bin/bash
#
# generate_configuration_files.sh
# A bash script to generate configuration files
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

# Generate satellite TLE file
python3 generate_sat_config_file.py ../../generate-ecis/configuration/flock-3k-3.tle ../configuration/
# Generate date-time.dat file
python3 generate_date_time_config_file.py ../configuration/ ../configuration/
# Generate time-step.dat and num-steps.dat for constellation config tool
for tlefile in ../configuration/*.tle # for loop is a hack, there should be 1 tle
do
  python3 generate_step_config_files.py $tlefile 3 0 0 0 10000000 ../configuration/
done
# Use constellation config tool to generate constellation configuration files
cd ../../generate-constellation-configuration/build/
CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ cmake ../source/
make
rm -rf CMakeFiles/
rm CMakeCache.txt cmake_install.cmake Makefile
./generate_constellation_configuration ../../generate-coms/configuration/ ../logs/
rm generate_constellation_configuration
cd ../scripts/
python3 generate_constellation_files.py 1.1 4096 20.0 215 ../../generate-coms/configuration/ ../logs/ ../../generate-coms/scripts/
rm ../logs/*.csv
cd ../../generate-coms/scripts/
cp orbit-spaced.dat ../configuration/constellation.dat
# Generate ground configuration files
python3 generate_gnd_config_files.py  87.0 0.0 12 90000 ../configuration/
python3 generate_gnd_config_files.py -87.0 0.0 12 91000 ../configuration/
# Generate transmitter configuration file
python3 generate_tx_config_file.py ../configuration/ tle 2.0 -1.0 6.0 8.15e9 20.0e6 ../configuration/
# Generate receiver configuration files
mkdir ../configuration/gnds/
for gndfile in ../configuration/gnd-*.dat # for loop is real, should be many gnds
do
  cp $gndfile ../configuration/gnds/
done
python3 generate_rx_config_file.py ../configuration/gnds/ dat 44.1 8.15e9 20.0e6 ../configuration/
rm -rf ../configuration/gnds/
# Generate time-step.dat and num-steps.dat for communication simulation
for tlefile in ../configuration/*.tle # for loop is a hack, there should be 1 tle
do
  python3 generate_step_config_files.py $tlefile 2 0 0 0 10000000 ../configuration/
done
