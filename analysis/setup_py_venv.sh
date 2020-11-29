#!/bin/bash
#
# setup_py_venv.sh
# A bash script that sets up a Python 3 virtual environment
#
# Usage: ./setup_py_venv.sh
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

python3 -m venv p3-env
source p3-env/bin/activate
python3 -m pip install matplotlib==3.3.3
deactivate
