#!/bin/bash
#
# setup_dependencies_mac.sh
# A bash script that sets up dependencies on macOS
#
# NOTE: RUN THE FOLLOWING COMMAND FROM THE TERMINAL BEFORE EXECUTING THIS SCRIPT
#
# xcode-select --install
#
# NOTE: IF USING 10.14 (Mojave) OR LATER, RUN THE FOLLOWING COMMANDS
#
# cd /Library/Developer/CommandLineTools/Packages/
# open .
# # Double-click the pkg file and accept the defaults 
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
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication with this
# software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

# Initialization
if [ -z "$1" ]
then
  echo "Usage: ./setup_dependencies_mac.sh /path/to/sw"
  exit 0
fi
DIR=$1
if [ ! -d "$DIR" ]
then
  mkdir $DIR
fi

# GCC 8.3.0 for C++17
cd $DIR/
if [ ! -d "$DIR/gcc-8.3.0" ]
then
  curl https://ftp.gnu.org/gnu/gcc/gcc-8.3.0/gcc-8.3.0.tar.gz -o gcc-8.3.0.tar.gz
  tar xzf gcc-8.3.0.tar.gz
  rm gcc-8.3.0.tar.gz
  curl https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2 -o gmp-6.1.2.tar.bz2
  tar xjf gmp-6.1.2.tar.bz2
  rm gmp-6.1.2.tar.bz2
  cp -rp gmp-6.1.2/ gcc-8.3.0/gmp/
  rm -rf gmp-6.1.2/
  curl https://www.mpfr.org/mpfr-current/mpfr-4.0.2.tar.gz -o mpfr-4.0.2.tar.gz
  tar xzf mpfr-4.0.2.tar.gz
  rm mpfr-4.0.2.tar.gz
  cp -rp mpfr-4.0.2/ gcc-8.3.0/mpfr/
  rm -rf mpfr-4.0.2/
  curl https://ftp.gnu.org/gnu/mpc/mpc-1.1.0.tar.gz -o mpc-1.1.0.tar.gz
  tar xzf mpc-1.1.0.tar.gz
  rm mpc-1.1.0.tar.gz
  cp -rp mpc-1.1.0/ gcc-8.3.0/mpc/
  rm -rf mpc-1.1.0/
fi
if [ ! -d "$DIR/gcc-8.3.0-build" ]
then
  mkdir gcc-8.3.0-build/
fi
if [ ! -d "$DIR/gcc-8.3.0-install" ]
then
  mkdir gcc-8.3.0-install/
  cd gcc-8.3.0-build/
  ../gcc-8.3.0/configure --disable-multilib --prefix=$DIR/gcc-8.3.0-install CC=clang CXX=clang++
  make -j $( sysctl -n hw.ncpu )
  make install
fi
