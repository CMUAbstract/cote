#!/bin/bash
#
# generate_contellation_tracks.sh
# A bash script to generate constellation tracks
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

./setup_p3_venv.sh
source p3-env/bin/activate
cd ../build/
CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ cmake ../source/
make
cp ../scripts/close-spaced.dat ../configuration/constellation.dat
./generate_constellation_tracks ../configuration/ ../logs/
python3 ../scripts/plot_ground_tracks.py ../logs/ ../scripts/close-spaced.pdf
cp ../scripts/frame-spaced.dat ../configuration/constellation.dat
./generate_constellation_tracks ../configuration/ ../logs/
python3 ../scripts/plot_ground_tracks.py ../logs/ ../scripts/frame-spaced.pdf
cp ../scripts/orbit-spaced.dat ../configuration/constellation.dat
./generate_constellation_tracks ../configuration/ ../logs/
python3 ../scripts/plot_ground_tracks.py ../logs/ ../scripts/orbit-spaced.pdf
deactivate
