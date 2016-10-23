#!/bin/sh

# get path of this script file
BASEDIR=$(dirname "$0")

# run cmake
cmake ${BASEDIR}

# run make
make -C ${BASEDIR}
