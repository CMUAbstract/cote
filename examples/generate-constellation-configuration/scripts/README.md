# Scripts

This directory contains scripts to support the program, e.g. scripts to generate
the configuration files.

## Directory Contents

* [generate_epoch_configuration_file.py](generate_epoch_configuration_file.py):
  Generates a `date-time.dat` simulation start file set to the latest epoch
  among all TLEs
* [generate_time_configuration_files.py](generate_time_configuration_files.py):
  Generates `num-steps.dat`, which indicates the number of simulation steps to
  perform, and `time-step.dat`, which indicates the time elapsed between
  simulation steps
* [generate_constellation_files.py](generate_constellation_files.py): Generates
  constellation configuration files
* [generate_constellation_configurations.sh](generate_constellation_configurations.sh):
  Combines all scripts in order to produce constellation configuration files
* [README.md](README.md): This document

## License

Written by Bradley Denby  
Other contributors: None

To the extent possible under law, the author(s) have dedicated all copyright and
related and neighboring rights to this work to the public domain worldwide. This
work is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication with this
work. If not, see <https://creativecommons.org/publicdomain/zero/1.0/>.
