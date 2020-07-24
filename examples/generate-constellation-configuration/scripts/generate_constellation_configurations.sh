#!/bin/bash
#
# generate_constellation_configurations.sh
# A bash script to generate constellation configuration files
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

python3 generate_epoch_configuration_file.py ../configuration/ ../configuration/
python3 generate_time_configuration_files.py ../configuration/flock-3p-15.tle 3 0 0 0 10000000 ../configuration/
cd ../build/
CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ cmake ../source/
make
./generate_constellation_configuration ../configuration/ ../logs/
cd ../scripts/
python3 generate_constellation_files.py 1.1 4096 20.0 215 ../configuration/ ../logs/ ../../generate-constellation-tracks/scripts/
