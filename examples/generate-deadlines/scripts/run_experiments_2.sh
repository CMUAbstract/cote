#!/bin/bash
# Usage: ./run_experiments_2.sh
#
# run_experiments_2.sh
# A bash script that runs the second half of the experiments
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

baseline=$( ps -u $(whoami) | sed 1d | wc -l )
./prep_config_files_2.sh
tles=$(python3 get_tles.py ./tles/ 2)
cd ../build
CC=$HOME/sw/gcc-8.3.0-install/bin/gcc CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ \
 LD_LIBRARY_PATH=$HOME/sw/gcc-8.3.0-install/lib64/ cmake ../source/
make
for tle in $tles
do
  ./generate_deadlines ../configuration/$tle/ ../logs/$tle/ &
  # if necessary, wait for cores to free
  while (( $(( $( ps -u $(whoami) | sed 1d | wc -l ) - $baseline )) > $(nproc) ))
  do
    sleep 1000
  done
done
