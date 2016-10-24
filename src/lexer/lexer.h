#pragma once


#include <iostream>
#include <fstream>


namespace cmpl
{

  class Lexer
  {
    public:
      Lexer(std::file &file);
      Token getNextToken();
      
    private:
      std::vector<Token> tokenArray;
  };

}
