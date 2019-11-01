#!/bin/bash
# Usage: ./prep_frame_spaced_scenarios.sh
#
# prep_frame_spaced_scenarios.sh
# A bash script that prepares all scenarios
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

# Generate sub-directories for configuration files and log files
sat_config=planet
for sat_count in 050 100 150 200 250
do
  for gnd_count in 002 004 006 008 010 020 040 060 080 100
  do
    # north/south polar ground stations
    gnd_config=ns
    if [ ! -d ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/ ]
    then
      mkdir ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    fi
    cp ./common-data/date-time.dat ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    cp ./common-data/time-step.dat ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    cp ./common-data/num-steps.dat ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    cp ./common-data/$sat_config-$sat_count.dat ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/constellation.dat
    cp ./common-data/$sat_config.tle ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    cp ./common-data/sensor.dat ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    python3 populate_tx.py ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/ tle 2.0 -1.0 6.0 8.15e9 20.0e6 ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    mkdir ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/temp/
    python3 populate_gnd_ring.py  87.0 0.0 $(( $((10#$gnd_count))/2 )) 90000 ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/temp/
    python3 populate_gnd_ring.py -87.0 0.0 $(( $((10#$gnd_count))/2 )) 91000 ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/temp/
    python3 populate_rx.py ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/temp/ dat 44.1 8.15e9 20.0e6 ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    cp ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/temp/*.dat ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/
    rm -rf ../configuration/$sat_config-$sat_count-$gnd_config-$gnd_count/temp/
    if [ ! -d ../logs/$sat_config-$sat_count-$gnd_config-$gnd_count ]
    then
      mkdir ../logs/$sat_config-$sat_count-$gnd_config-$gnd_count
    fi
  done
done
