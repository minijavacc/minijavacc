#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace cmpl {

  class Compiler
  {
    public:
      int echo(std::ifstream &file);
      
      void output(std::string &msg);
      void error(std::string &msg);
  };

}
