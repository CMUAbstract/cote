#!/bin/bash
#
# generate_figure.sh
# A bash script to generate a ground track figure
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

cd ../build/
CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ cmake ../source/
make
./generate_ground_tracks ../configuration/ ../logs/
cd ../scripts/
./setup_p3_venv.sh
source p3-env/bin/activate
python3 plot_ground_tracks.py ../logs/
deactivate
