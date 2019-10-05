#!/bin/bash
#
# generate_contellation_tracks.sh
# A bash script to generate constellation tracks
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
