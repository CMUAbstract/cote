# Scripts

This directory contains scripts.

## Directory Contents

* [setup_dependencies.sh](setup_dependencies.sh): Run this script to setup the
  library dependencies. CMake must be installed separately.
* [setup_dependencies_mac.sh](setup_dependencies_mac.sh): Run this script to
  setup the library dependencies on macOS Catalina or later. CMake must be
  installed separately.
* [generate_polar_norad_ids.py](generate_polar_norad_ids.py): Generates a list
  of NORAD IDs for satellites in polar orbits at different altitudes
* [generate_step_config_files.py](generate_step_config_files.py): Given a TLE
  file, the number of revolutions to simulate, and the desired simulation time
  steps, this script outputs the appropriate `num-steps.dat` and `time-step.dat`
  configuration files
* [generate_small_step_config_files.py](generate_small_step_config_files.py):
  Use this script to generate `num-steps.dat` and `time-step.dat` configuration
  files when the time step is very small compared to the time for revolutions
* [README.md](README.md): This document

## License

Written by Bradley Denby  
Other contributors: None

To the extent possible under law, the author(s) have dedicated all copyright and
related and neighboring rights to this work to the public domain worldwide. This
work is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication with this
work. If not, see <https://creativecommons.org/publicdomain/zero/1.0/>.
