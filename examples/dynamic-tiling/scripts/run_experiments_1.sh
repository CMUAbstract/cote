#!/bin/bash
# Usage: ./run_experiments_1.sh
#
# run_experiments_1.sh
# A bash script that runs the first half of the experiments
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
./prep_config_files_1.sh
tles=$(python3 get_tles.py ./tles/ 1)
cd ../build
CC=$HOME/sw/gcc-8.3.0-install/bin/gcc CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ \
 LD_LIBRARY_PATH=$HOME/sw/gcc-8.3.0-install/lib64/ cmake ../source/
make
for tle in $tles
do
  ./dynamic_tiling ../configuration/$tle/ ../logs/$tle/ &
  # if necessary, wait for cores to free
  while (( $(( $( ps -u $(whoami) | sed 1d | wc -l ) - $baseline )) > $(nproc) ))
  do
    sleep 1000
  done
done
