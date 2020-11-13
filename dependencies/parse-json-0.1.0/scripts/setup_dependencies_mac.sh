#!/bin/bash
#
# setup_dependencies_mac.sh
# A bash script that sets up dependencies on macOS Catalina (10.15)
#
# NOTE: RUN THE FOLLOWING COMMAND FROM THE TERMINAL BEFORE EXECUTING THIS SCRIPT
#
# xcode-select --install
#
# Usage: ./setup_dependencies_mac.sh /path/to/sw
# Arguments:
#  - /path/to/sw: The path to the sw directory where the dependencies will be
#    placed
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
  echo "Usage: ./setup_dependencies_mac.sh /path/to/sw"
  echo "/path/to/sw specifies where gcc will be placed"
  exit 0
fi
DIR=$1
if [ ! -d "$DIR" ]
then
  mkdir $DIR
fi

# GCC 9.2.0 for C++17
cd $DIR/
if [ ! -d "$DIR/gcc-9.2.0" ]
then
  curl https://ftp.gnu.org/gnu/gcc/gcc-9.2.0/gcc-9.2.0.tar.gz -o gcc-9.2.0.tar.gz
  tar xzf gcc-9.2.0.tar.gz
  rm gcc-9.2.0.tar.gz
  curl https://raw.githubusercontent.com/Homebrew/formula-patches/b8b8e65e/gcc/9.2.0-catalina.patch -o 9.2.0-catalina.patch
  cd gcc-9.2.0
  ./contrib/download_prerequisites
  cd fixincludes/
  patch < ../../9.2.0-catalina.patch
  rm ../../9.2.0-catalina.patch
  cd ../../
fi
if [ ! -d "$DIR/gcc-9.2.0-build" ]
then
  mkdir gcc-9.2.0-build/
fi
if [ ! -d "$DIR/gcc-9.2.0-install" ]
then
  mkdir gcc-9.2.0-install/
  cd gcc-9.2.0-build/
  ../gcc-9.2.0/configure --disable-multilib --prefix=$DIR/gcc-9.2.0-install \
   --with-sysroot=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/
  make -j $( sysctl -n hw.ncpu )
  make install
  cd ../
  cp -rp gcc-9.2.0-install/ gcc-8.3.0-install/
  cp -rp gcc-9.2.0-install/ gcc/
fi
