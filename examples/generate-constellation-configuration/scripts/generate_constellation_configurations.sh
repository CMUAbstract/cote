#!/bin/bash
#
# generate_constellation_configurations.sh
# A bash script to generate constellation configuration files
#
# Written by Bradley Denby
# Other contributors: None
#
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication with this
# software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

python3 generate_epoch_configuration_file.py ../configuration/ ../configuration/
python3 generate_time_configuration_files.py ../configuration/flock-3p-15.tle 3 0 0 0 10000000 ../configuration/
cd ../build/
CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ cmake ../source/
make
./generate_constellation_configuration ../configuration/ ../logs/
cd ../scripts/
python3 generate_constellation_files.py 1.1 4096 20.0 215 ../configuration/ ../logs/ ../../generate-constellation-tracks/scripts/
