#!/bin/bash
# Usage: ./clean_crosslink_los.sh
#
# clean_crosslink_los.sh
# A bash script that cleans the example
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cd ../build/
rm -rf CMakeFiles/
rm CMakeCache.txt cmake_install.cmake crosslink_los Makefile
cd ../configuration/
rm *.sat *.dat
cd ../logs/
rm *.csv
cd ../scripts/
rm *.txt
