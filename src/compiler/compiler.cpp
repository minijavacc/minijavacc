#include "compiler.hpp"


#include <iostream>
#include <fstream>


namespace COMPILER
{
  
  int Compiler::echo(std::ifstream &file)
  {
    std::string line;
    
    while(std::getline(file, line))
    {
      std::cout << line << "\n";
    }
    
    return 0;
  }

}
