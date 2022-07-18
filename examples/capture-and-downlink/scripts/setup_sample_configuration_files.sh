#!/bin/bash
# Usage: ./setup_sample_configuration_files.sh
#
# setup_sample_configuration_files.sh
# A bash script that sets up sample configuration files
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cp ./sample-configuration-files/date-time.dat ../configuration/
cp ./sample-configuration-files/time-step.dat ../configuration/
cp ./sample-configuration-files/num-steps.dat ../configuration/
cp ./sample-configuration-files/sat-0000051003-flock-4x-22.sat ../configuration/
cp ./sample-configuration-files/sensor-sat-0000051003.dat ../configuration/
cp ./sample-configuration-files/tx-sat-0000051003.dat ../configuration/
cp ./sample-configuration-files/gnd-0000064019-iceland.gnd ../configuration/
cp ./sample-configuration-files/rx-gnd-0000064019.dat ../configuration/
