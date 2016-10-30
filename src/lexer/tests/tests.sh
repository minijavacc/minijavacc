#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
CLEAR='\033[0m'

# Assume script resides inside src/lexer/tests directory and is always called from there

BASEDIR='./src/lexer/tests'
cd ../../..

lexer='./run'

test_files=( 'edge_cases' 'prog1' 'test1' 'test2' 'test3' 'test4' 'test5' 'while' )
error_files=( 'errors' )



success=true



for i in "${test_files[@]}"
do
  $lexer --lextest "${BASEDIR}/"$i"_in.txt" > "${BASEDIR}/"$i".out"

  diff "${BASEDIR}/"$i".out" "${BASEDIR}/"$i"_out.txt" > o

  lo=`cat o | wc -l`

  if [ $lo -ne 0  ]; then
    echo "${RED}"$i" tests failed (stdout).${CLEAR} Diff:"
    cat o
    # cat "${BASEDIR}/"$i".out"
    echo ''
    success=false
  fi

  rm "${BASEDIR}/"$i".out"
  rm o
done



for i in "${error_files[@]}"
do
  $lexer --lextest "${BASEDIR}/"$i"_in.txt" 2> "${BASEDIR}/"$i".err"

  le=`cat "${BASEDIR}/"$i".err" | grep error | wc -l`

  if [ $le -lt 1  ]; then
    echo -e "${RED}"$i" tests failed (stderr).${CLEAR} Should produce error but does not."
    success=false
  fi

  rm "${BASEDIR}/"$i".err"
done



cd $BASEDIR
./single_test_cases.sh



if [ "$success" = true ]; then
  echo -e "${GREEN}All lexer tests passed${CLEAR}"
  exit 0
else
  exit 1
fi