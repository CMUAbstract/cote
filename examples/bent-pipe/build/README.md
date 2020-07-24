# Compilation and Testing

Compilation is managed by CMake to facilitate straightforward compilation on
multiple platforms. Ensure that CMake is installed before proceeding. A
CMakeLists.txt file should be available in the test directory.

## Compile

Execute the following commands.

```bash
cd $HOME/git-repos/cote/examples/bent-pipe/build/
CXX=$HOME/sw/gcc-8.3.0-install/bin/g++ cmake ../source/
make
```

## Run

After compilation, run the executable.

```bash
./bent_pipe ../configuration/ ../logs/
```

## License

Written by Bradley Denby  
Other contributors: None

See the top-level LICENSE file for the license.
