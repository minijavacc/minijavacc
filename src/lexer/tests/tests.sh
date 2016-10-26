#!/bin/sh

BASEDIR=$(dirname "$(readlink "$0")")



lexer=${BASEDIR}/tokenizer




cat edge_cases_in.txt | $lexer > ${BASEDIR}/edge_cases.out 2> ${BASEDIR}/edge_cases.err

diff ${BASEDIR}/edge_cases.out ${BASEDIR}/edge_cases_out.txt > o
diff ${BASEDIR}/edge_cases.err ${BASEDIR}/edge_cases_err.txt > e

lo=`cat o | wc -l`
le=`cat e | wc -l`

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






cat errors_in.txt | $lexer > ${BASEDIR}/errors.out 2> ${BASEDIR}/errors.err

diff ${BASEDIR}/errors.out ${BASEDIR}/errors_out.txt > o
diff ${BASEDIR}/errors.err ${BASEDIR}/errors_err.txt > e

lo=`cat o | wc -l`
le=`cat e | wc -l`

if [ $lo -ne 0  ]; then
  echo "Error tests failed (stdout)"
  exit 1
fi

if [ $le -ne 0  ]; then
  echo "Error tests failed (stderr)"
  exit 1
fi

# rm ${BASEDIR}/errors.out
rm ${BASEDIR}/errors.err





cat Prog1.java | $lexer > ${BASEDIR}/prog1.out

diff ${BASEDIR}/prog1.out ${BASEDIR}/Prog1.java.lex > o

lo=`cat o | wc -l`

if [ $lo -ne 0  ]; then
  echo "Error tests failed (stdout)"
  exit 1
fi

rm ${BASEDIR}/Prog1.java.lex






rm e
rm o

exit 0