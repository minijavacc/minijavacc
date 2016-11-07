module ParserTests where

import Parser
import Lexer

testProgram = parse $ tokenize "class A{ public int test(int foo, boolean bla) {;} }"
testParameters = parseParameters $ tokenize "int foo, boolean bla"