#!/bin/bash

ops=(
"!="
"!"
"("
")"
"*="
"*"
"++"
"+="
"+"
","
"-="
"--"
"-"
"."
"/="
"/"
":"
";"
"<<="
"<<"
"<="
"<"
"=="
"="
">="
">>="
">>>="
">>>"
">>"
">"
"?"
"%="
"%"
"&="
"&&"
"&"
"["
"]"
"^="
"^"
"{"
"}"
"~"
"|="
"||"
"|"
)

op_names=(
T_O_EXCLM_EQUAL
T_O_EXCLM
T_O_LPAREN
T_O_RPAREN
T_O_STAR_EQUAL
T_O_STAR
T_O_PLUS_PLUS
T_O_PLUS_EQUAL
T_O_PLUS
T_O_COMMA
T_O_MINUS_EQUAL
T_O_MINUS_MINUS
T_O_MINUS
T_O_DOT
T_O_SLASH_EQUAL
T_O_SLASH
T_O_COLON
T_O_SEMICOLON
T_O_LESS_LESS_EQUAL
T_O_LESS_LESS
T_O_LESS_EQUAL
T_O_LESS
T_O_EQUAL_EQUAL
T_O_EQUAL
T_O_MORE_EQUAL
T_O_MORE_MORE_EQUAL
T_O_MORE_MORE_MORE_EQUAL
T_O_MORE_MORE_MORE
T_O_MORE_MORE
T_O_MORE
T_O_QUESTM
T_O_PERCENT_EQUAL
T_O_PERCENT
T_O_AND_EQUAL
T_O_AND_AND
T_O_AND
T_O_LBRACK
T_O_RBRACK
T_O_CARET_EQUAL
T_O_CARET
T_O_LBRACE
T_O_RBRACE
T_O_TILDE
T_O_PIPE_EQUAL
T_O_PIPE_PIPE
T_O_PIPE
)



RED='\033[0;31m'
GREEN='\033[0;32m'
CLEAR='\033[0m'

# Assume script resides inside src/lexer/tests directory and is always called from there

BASEDIR='tests/lexer'
lexer='./run'


success=true


    
for ((i=0; i<${#ops[*]}; i++));
do
  echo -e "${ops[$i]}" > "single_${op_names[$i]}_in.txt"
  echo -e "${ops[$i]}\nEOF" > "single_${op_names[$i]}_out.txt"
  
  $lexer --lextest "single_${op_names[$i]}_in.txt" > "single_${op_names[$i]}.out"
  
  diff "single_${op_names[$i]}.out" "single_${op_names[$i]}_out.txt" > o
  
  lo=`cat o | wc -l`
  
  if [ $lo -ne 0 ]; then
    echo -e "${RED}${op_names[$i]} tests failed (stdout).${CLEAR} Diff:"
    cat o
    # cat "${BASEDIR}/"$i".out"
    echo ''
    success=false
  fi

  rm "single_${op_names[$i]}.out"
  rm "single_${op_names[$i]}_in.txt"
  rm "single_${op_names[$i]}_out.txt"
  rm o
done


if [ "$success" = true ]; then
  echo -e "${GREEN}All single token lexer tests passed${CLEAR}"
  exit 0
else
  exit 1
fi
