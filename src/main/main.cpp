#include "compiler.hpp"
#include "inputparser.hpp"


#include <iostream>
#include <fstream>


using namespace COMPILER;


int main(int argc, char* argv[])
{
  Compiler c;
  InputParser input(argc, argv);
  
  if (input.cmdOptionExists("--echo"))
  {
    const std::string &filename = input.getCmdOption("--echo");
    if (!filename.empty())
    {
      std::ifstream file;
      
      file.open(filename);
      c.echo(file);
      file.close();
    }
  }
  
  return 0;
}