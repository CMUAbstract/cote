#!/bin/bash
# Usage: ./run_frame_spaced_scenarios.sh
#
# run_frame_spaced_scenarios.sh
# A bash script that runs all scenarios
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

baseline=$( ps -u $(whoami) | sed 1d | wc -l )
./prep_frame_spaced_scenarios.sh
cd ../build/
CC=$HOME/sw/gcc-8.3.0-install/bin/gcc CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ \
 LD_LIBRARY_PATH=$HOME/sw/gcc-8.3.0-install/lib64/ cmake ../source/
make
sat_config=planet
for sat_count in 050 100 150 200 250
do
  for gnd_count in 002 004 006 008 010 020 040 060 080 100
  do
    # north/south polar ground stations
    gnd_config=ns
    ./frame_spaced ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/ \
                   ../logs/$sat_config-$sat_count-$gnd_config-$gnd_count/ &
    # if necessary, wait for cores to free
    while (( $(( $( ps -u $(whoami) | sed 1d | wc -l ) - $baseline )) > $(nproc) ))
    do
      sleep 1000
    done
  done
done
