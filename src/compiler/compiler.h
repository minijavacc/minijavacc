#pragma once

#include <iostream>
#include <fstream>

namespace cmpl {

  class Compiler
  {
    public:
      int echo(std::ifstream &file);
  };

}
