#pragma once

#include "token.h"
#include "stringtable.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

namespace cmpl
{

  class Lexer
  {
    public:
      Lexer(StringTable &stringTable) : stringTable(stringTable) {};
      void run(std::ifstream &inputFile);
      Token getNextToken();
      
    private:
      void insertToken(std::unique_ptr<Token> token);
      
      StringTable &stringTable;
      std::vector<std::unique_ptr<Token>> tokenArray;
  };
  
  class SyntaxError : public std::exception {};

}
