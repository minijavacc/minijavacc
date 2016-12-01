#pragma once

#include "token.h"
#include "stringtable.h"
#include "compiler.h"

#include <deque>
#include <iostream>
#include <fstream>
#include <memory>

namespace cmpl
{

  class Lexer
  {
    public:
      Lexer(std::ifstream &inputFile);
      void run();
      bool getNextToken(std::unique_ptr<Token> &t);
      bool putBackToken(std::unique_ptr<Token> &t);
      bool hasNextToken() const;
      
    private:
      inline void insertToken(std::unique_ptr<Token> token);
      inline std::istream& getNextChar(std::ifstream &inputFile, char& currentChar, unsigned int& line, unsigned int& column);
      inline void error(char currentChar, unsigned int line, unsigned int column);
      
      std::deque<std::unique_ptr<Token>> tokenArray;
      std::ifstream &inputFile;
  };
  
  class SyntaxError : public std::runtime_error
  {
    public:
      SyntaxError(const std::string& err, unsigned int line, unsigned int column)
        : std::runtime_error(err), line(line), column(column) { }
      unsigned int line;
      unsigned int column;
  };

}
