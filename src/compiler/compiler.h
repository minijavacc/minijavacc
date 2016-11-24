#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace cmpl {

  class Compiler
  {
  public:
      static int echo(std::ifstream &file);
      static int lextest(std::ifstream &file);
      static int parsetest(std::ifstream &file);
      static int printast(std::ifstream &file);
      static int semcheck(std::ifstream &file);
      static int creategraph(std::ifstream &file);
      
      static std::string sourcePreview(std::ifstream &file, unsigned int line, unsigned int column);
  };

}
