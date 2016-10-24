#include "compiler.h"

#include <iostream>
#include <fstream>

using namespace cmpl;

int echo(std::ifstream &file)
{
  std::string line;
  
  while(std::getline(file, line))
  {
    std::cout << line << "\n";
  }
  
  return 0;
}

