#!/bin/sh

# get path of this script file
BASEDIR=$(dirname "$(readlink -e "$0")")
echo $BASEDIR

# run cmake
(cd ${BASEDIR} && cmake ${BASEDIR})
ret=$?
if [ $ret -ne 0 ]; then
  exit 1
fi

# run make
(cd ${BASEDIR} && make -C ${BASEDIR})
ret=$?
if [ $ret -ne 0 ]; then
  exit 1
fi

exit 0
