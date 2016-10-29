#!/bin/sh

RED='\033[0;31m'
CLEAR='\033[0m'

BASEDIR=$(pwd)

cd ../../..

lexer=./run

test_files[0]="edge_cases"
test_files[2]="prog1"
test_files[3]="test1"
test_files[4]="test2"
test_files[5]="test3"
test_files[5]="test4"
test_files[5]="test5"

error_files[0]="errors"



success=true



for i in "${test_files[@]}"
do
  $lexer --lextest "${BASEDIR}/"$i"_in.txt" > "${BASEDIR}/"$i".out"

  diff "${BASEDIR}/"$i".out" "${BASEDIR}/"$i"_out.txt" > o

  lo=`cat o | wc -l`

  if [ $lo -ne 0  ]; then
    echo "${RED}"$i" tests failed (stdout).${CLEAR} Diff:"
    cat o
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
    echo "${RED}"$i" tests failed (stderr).${CLEAR} Should produce error but does not."
    success=false
  fi

  rm "${BASEDIR}/"$i".err"
done




if [ "$success" = true ]; then
  exit 0
else
  exit 1
fi