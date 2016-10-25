#pragma once

#include "token.h"
#include "stringtable.h"

#include <vector>
#include <iostream>
#include <fstream>

namespace cmpl
{

  class Lexer
  {
    public:
      Lexer();
      void run(std::ifstream inputFile);
      Token getNextToken();
      
    private:
      StringTable &stringTable;
      
      std::vector<Token> tokenArray;
  };
  
  class SyntaxError : public std::exception {};

}
