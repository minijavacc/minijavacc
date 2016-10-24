#pragma once

#include "token.h"

#include <vector>
#include <iostream>
#include <fstream>

namespace cmpl
{

  class Lexer
  {
    public:
      Lexer();
      void run();
      Token getNextToken();
      
    private:
      std::ifstream &inputFile;
      
      // TODO: stringtable
      // - maps std::string to IdentifierTokenId
      
      std::vector<Token> tokenArray;
  };

}
