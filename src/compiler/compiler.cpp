#include "compiler.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace cmpl;

int Compiler::echo(std::ifstream &file)
{
  std::string line;
  
  while(std::getline(file, line))
  {
    std::cout << line << "\n";
  }
  
  return 0;
}

void Compiler::output(std::string msg)
{
  std::cout << msg << "\n";
}

void Compiler::error(std::string msg)
{
  std::cerr << msg << "\n";
}
