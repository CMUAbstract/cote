# Scripts

This directory contains scripts to support the program, e.g. scripts to generate
the configuration files.

To generate the configuration files, run
[generate_configuration_files.sh](generate_configuration_files.sh):

```bash
./generate_configuration_files.sh
```

## Directory Contents

* [generate_sat_config_file.py](generate_sat_config_file.py): Generates the
  satellite TLE configuration file. Attempts to retrieve a fresh TLE from the
  web.
* [generate_date_time_config_file.py](generate_date_time_config_file.py):
  Generates the date and time configuration file based on the satellite TLE.
* [generate_step_config_files.py](generate_step_config_files.py): Generates the
  time step and number of steps configuration files.
* [generate_gnd_config_files.py](generate_gnd_config_files.py): Generates ground
  station configuration files.
* [generate_tx_config_file.py](generate_tx_config_file.py): Generates
  transmitter configuration files.
* [generate_rx_config_file.py](generate_rx_config_file.py): Generates receiver
  configuration files.
* [generate_configuration_files.sh](generate_configuration_files.sh): Executes
  all of the scripts in the appropriate order in order to generate the necessary
  configuration files.
* [README.md](README.md): This document

## License

Written by Bradley Denby  
Other contributors: None

See the top-level LICENSE file for the license.
