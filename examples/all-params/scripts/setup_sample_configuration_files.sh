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

cd ../../../scripts/gen-config/
python3 generate_orbit_spaced_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/configuration/
python3 generate_date_time_file.py ../../examples/all-params/configuration/ \
 ../../examples/all-params/configuration/
python3 generate_time_step_file.py 0 0 0 1000000 \
 ../../examples/all-params/configuration/
python3 generate_num_steps_file.py 3.0 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/configuration/time-step.dat \
 ../../examples/all-params/configuration/
python3 generate_solar_array_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/solar-array.dat \
 ../../examples/all-params/configuration/
python3 generate_capacitor_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/capacitor.dat \
 ../../examples/all-params/configuration/
python3 generate_adacs_sm_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/adacs-sm-sat.dat \
 ../../examples/all-params/configuration/
python3 generate_camera_sm_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/camera-sm-sat.dat \
 ../../examples/all-params/configuration/
python3 generate_computer_sm_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/computer-sm-sat.dat \
 ../../examples/all-params/configuration/
python3 generate_rx_sm_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/rx-sm-sat.dat \
 ../../examples/all-params/configuration/
python3 generate_tx_sm_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/tx-sm-sat.dat \
 ../../examples/all-params/configuration/
python3 generate_rx_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/rx-sat.dat \
 ../../examples/all-params/configuration/
python3 generate_tx_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/tx-sat.dat \
 ../../examples/all-params/configuration/
python3 generate_sensor_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/sensor.dat \
 ../../examples/all-params/configuration/
python3 generate_program_sat_files.py 24 \
 ../../examples/all-params/scripts/sample-configuration-files/starlink-1082-4493800000.tle \
 ../../examples/all-params/scripts/sample-configuration-files/program.dat \
 ../../examples/all-params/configuration/
cp ../../examples/all-params/scripts/sample-configuration-files/constellation-0000000001.json \
 ../../examples/all-params/configuration/
python3 generate_gnd_files.py 24 1 \
 ../../examples/all-params/scripts/sample-configuration-files/universities.csv \
 ../../examples/all-params/configuration/
python3 generate_rx_gnd_files.py \
 ../../examples/all-params/configuration/ \
 ../../examples/all-params/scripts/sample-configuration-files/rx-gnd.dat \
 ../../examples/all-params/configuration/
python3 generate_tx_gnd_files.py \
 ../../examples/all-params/configuration/ \
 ../../examples/all-params/scripts/sample-configuration-files/tx-gnd.dat \
 ../../examples/all-params/configuration/





















