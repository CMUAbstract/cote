#!/bin/bash
# Usage: ./prep_config_files_2.sh
#
# prep_config_files_2.sh
# A bash script that prepares the second half of the configuration files
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

# Generate sub-directories for configuration files and log files
tles=$(python3 get_tles.py ./tles/ 2)
for tle in $tles
do
  if [ ! -d "../configuration/$tle" ]
  then
    mkdir ../configuration/$tle
  fi
  cp tles/$tle.txt ../configuration/$tle/$tle.tle
  python3 generate_epoch_configuration_file.py ../configuration/$tle/ ../configuration/$tle/
  python3 generate_small_step_config_files.py ../configuration/$tle/$tle.tle 2 100000 ../configuration/$tle/
  cp sample-configuration-files/sensor.dat ../configuration/$tle/
  cp sample-configuration-files/neural-net.dat ../configuration/$tle/
  if [ ! -d "../logs/$tle" ]
  then
    mkdir ../logs/$tle
  fi
done
