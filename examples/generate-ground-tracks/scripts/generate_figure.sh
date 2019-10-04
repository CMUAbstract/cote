#!/bin/bash
#
# generate_figure.sh
# A bash script to generate a ground track figure
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

cd ../build/
CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ cmake ../source/
make
./generate_ground_tracks ../configuration/ ../logs/
cd ../scripts/
./setup_p3_venv.sh
source p3-env/bin/activate
python3 plot_ground_tracks.py ../logs/
deactivate
