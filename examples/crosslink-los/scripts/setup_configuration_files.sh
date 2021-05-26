#!/bin/bash
# Usage: ./setup_configuration_files.sh
#
# setup_configuration_files.sh
# A bash script that sets up configuration files
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

#PAGE=starlink.txt
PAGE=spire.txt
wget -O $PAGE http://celestrak.com/NORAD/elements/$PAGE
python3 generate_sat_files.py $PAGE ../configuration/
python3 generate_date_time_file.py ../configuration/ ../configuration/
python3 generate_time_step_file.py 0 1 0 0 ../configuration/
python3 generate_num_steps_file.py 0.5 $PAGE ../configuration/time-step.dat \
 ../configuration/
