#!/bin/sh

# get path of this script file
BASEDIR=$(dirname "$(readlink "$0")")

cd "${BASEDIR}"/hs
ghc -o parser Main.hs

exit 0
