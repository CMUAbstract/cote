#!/bin/bash
#
# setup_dependencies.sh
# A bash script that sets up dependencies
#
# Usage: ./setup_dependencies.sh /path/to/sw
# Arguments:
#  - /path/to/sw: The path to the sw directory where the dependencies will be
#    installed
# Outputs:
#  - Compiled software dependencies placed in the specified directory
#
# Written by Bradley Denby
# Other contributors: None
#
# See the top-level LICENSE file for the license.

# Initialization
if [ -z "$1" ]
then
  echo "Usage: ./setup_dependencies.sh /path/to/sw"
  exit 0
fi
DIR=$1
if [ ! -d "$DIR" ]
then
  mkdir $DIR
fi

# GCC 9.3.0 for C++17
cd $DIR/
if [ ! -d "$DIR/gcc-9.3.0" ]
then
  wget https://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-9.3.0.tar.gz
  tar xzf gcc-9.3.0.tar.gz
  rm gcc-9.3.0.tar.gz
  cd $DIR/gcc-9.3.0/
  ./contrib/download_prerequisites
  cd $DIR/
fi
if [ ! -d "$DIR/gcc-9.3.0-build" ]
then
  mkdir $DIR/gcc-9.3.0-build/
fi
if [ ! -d "$DIR/gcc-9.3.0-install" ]
then
  mkdir $DIR/gcc-9.3.0-install/
  cd $DIR/gcc-9.3.0-build/
  $DIR/gcc-9.3.0/configure --disable-multilib --prefix=$DIR/gcc-9.3.0-install
  make -j $(nproc)
  make install
fi
