#pragma once

#include "token.h"
#include "stringtable.h"
#include "compiler.h"

#include <queue>
#include <iostream>
#include <fstream>
#include <memory>

namespace cmpl
{

  class Lexer
  {
    public:
      Lexer();
      void run(std::ifstream &inputFile);
      bool getNextToken(std::unique_ptr<Token> &t);
      bool hasNextToken() const;
      
    private:
      inline void insertToken(std::unique_ptr<Token> token);
      
      StringTable stringTable;
      std::queue<std::unique_ptr<Token>> tokenArray;
  };
  
  class SyntaxError : public std::exception {};

}
