#!/bin/bash
# Usage: ./generate_3d_scatter_plots.sh
#
# generate_3d_scatter_plots.sh
# A bash script that generates 3d scatter plots
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

if [ ! -d "../p3-env" ]
then
  cd ../
  ./setup_py_venv.sh
  cd ./all-params/
fi
source ../p3-env/bin/activate
python3 generate_3d_scatter_plots.py \
 ../../examples/all-params/logs/ ./3d-scatter-charts/
deactivate
pdftk ./3d-scatter-charts/*.pdf cat output \
 ./3d-scatter-charts/3d-scatter-charts.pdf
