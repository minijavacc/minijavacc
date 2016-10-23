#!/bin/sh

# get path of this script file
BASEDIR=$(dirname "$0")

# run compiler with given arguments
${BASEDIR}/bin/compiler "$@"
