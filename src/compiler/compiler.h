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
      
      static void output(std::string msg);
      static void error(std::string msg);
  };

}
