# Orbital Edge Computing Simulation Software

This software provides classes and utilities for simulating orbital edge
computing. The software namespace, `cote`, stands for "computing on the edge."
A cote is a shelter for carrier pigeons.

See our Best Paper from ASPLOS'20:
[https://brandonlucia.com/pubs/oec-asplos2020.pdf](https://brandonlucia.com/pubs/oec-asplos2020.pdf)

**Current version**: 1.0.0

* This software uses [semantic versioning](http://semver.org).

**Dependencies**

* A C++17 compiler (for `std::filesystem`)
* CMake for building Makefiles

```bash
sudo apt install build-essential cmake
cd $HOME/git-repos/cote/scripts
./setup_dependencies.sh $HOME/sw
```

NOTE: In some cases, you may observe some errors in the output of the
`setup_dependencies.sh` script. So long as `gcc-8.3.0-install/bin/` contains
`g++`, then the setup script is successful.

**Some Usage Notes**

For immediate results, run the `setup_dependencies.sh` script and then run any
`example` program. When running an example program, you must build the program
and, depending on the example, you may need to copy the sample configuration
files into the configuration folder; if so, there is a script to do this for you
in the corresponding scripts directory.

There is a README file in pretty much every directory. The repository is
designed to be more or less self-documenting, with any questions that may arise
answered in a nearby README file or, in the case of software and scripts,
in-file comments.

`cote` programs ingest a handful of configuration files (using the C++17
filesystem feature); the configuration files are pretty ad-hoc at the moment and
could eventually be well-defined and collected into a single, well-formatted
JSON file for example. For now, follow the sample configuration files located in
the sample directory in the `scripts` folder of example programs.

Because program configurations are specified in configuration files, they can be
changed without needing to recompile the programs in order to try new
configurations and scenarios.

You will eventually want satellite TLEs from an up-to-date source, e.g.
`http://celestrak.com/NORAD/elements/`. TLEs are only accurate for a few weeks
(actual duration depends on the satellite), so when specifying the start date
and time of a `cote` simulation, you should choose something close to the TLE
epoch. Two scripts exist to generate these files for you:
`cote/scripts/generate_step_config_files.py` and 
`cote/scripts/generate_small_step_config_files.py`. You should usually use
`generate_small_step_config_files.py` unless your simulation will make large
(one second or greater) time steps between iterations.

`cote` has been tested on Linux and macOS but not Windows; you may want to use
WSL on Windows. `cote` is extremely reliable and well-tested but we don't have
any CI so ping me if you notice some silly snag somewhere.

We have a CONTRIBUTING document but currently don't enforce it because most
people just can't handle it.

## Directory Contents

* [examples](examples/README.md): Example programs
* [references](references/README.md): Reference documents
* [scripts](scripts/README.md): Useful scripts, e.g. for setting up dependencies
* [software](software/README.md): `cote` classes, utilities, etc.
* [README.md](README.md): This document

## License

Written by Bradley Denby  
Other contributors: None

See the top-level LICENSE file for the license.
