#include "lexer.h"
#include "token.h"
#include "compiler.h"

#include <iostream>
#include <fstream>

using namespace cmpl;


void Lexer::run()
{
  if (!inputFile)
    throw std::invalid_argument("no valid input file given");
  
  
}
