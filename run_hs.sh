#!/bin/sh

# get path of this script file
BASEDIR=$(dirname "$(readlink "$0")")

# run compiler with given arguments
${BASEDIR}/hs/parser "$@"