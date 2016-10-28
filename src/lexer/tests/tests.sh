#!/bin/sh

BASEDIR=$(pwd)

cd ../../..

lexer=./run




$lexer --lextest ${BASEDIR}/edge_cases_in.txt > ${BASEDIR}/edge_cases.out 2> ${BASEDIR}/edge_cases.err

diff ${BASEDIR}/edge_cases.out ${BASEDIR}/edge_cases_out.txt > o

lo=`cat o | wc -l`
le=`cat ${BASEDIR}/edge_cases.err | wc -l`

if [ $lo -ne 0  ]; then
  echo "Edge case tests failed (stdout)"
  exit 1
fi

if [ $le -ne 0  ]; then
  echo "Edge case tests failed (stderr)"
  exit 1
fi

rm ${BASEDIR}/edge_cases.out
rm ${BASEDIR}/edge_cases.err






$lexer --lextest ${BASEDIR}/errors_in.txt > ${BASEDIR}/errors.out 2> ${BASEDIR}/errors.err

diff ${BASEDIR}/errors.out ${BASEDIR}/errors_out.txt > o
cat ${BASEDIR}/errors.err | grep error > e

lo=`cat o | wc -l`
le=`cat e | wc -l`

if [ $lo -ne 0  ]; then
  echo "Error tests failed (stdout)"
  exit 1
fi

if [ $le -lt 1  ]; then
  echo "Error tests failed (stderr)"
  exit 1
fi

rm ${BASEDIR}/errors.out
rm ${BASEDIR}/errors.err





$lexer --lextest ${BASEDIR}/Prog1.java > ${BASEDIR}/prog1.out

diff ${BASEDIR}/prog1.out ${BASEDIR}/Prog1.java.lex > o

lo=`cat o | wc -l`

if [ $lo -ne 0  ]; then
  echo "Prog1 tests failed (stdout)"
  exit 1
fi

# rm ${BASEDIR}/prog1.out






rm e
rm o

exit 0