#!/bin/bash
#
# setup_p3_venv.sh
# A bash script to set up Python 3 virtual environment
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

python3 -m venv p3-env
source p3-env/bin/activate
python3 -m pip install --upgrade pip
python3 -m pip install --upgrade setuptools
python3 -m pip install wheel==0.33.6
python3 -m pip install cython==0.29.13
python3 -m pip install pyshp==2.1.0
python3 -m pip install numpy==1.17.2
python3 -m pip install matplotlib==3.1.1
python3 -m pip install shapely==1.6.4
python3 -m pip install cartopy==0.17.0
python3 -m pip install pykdtree==1.3.1
python3 -m pip install scipy==1.3.1
deactivate
