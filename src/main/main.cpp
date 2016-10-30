#include "compiler.h"
#include "inputparser.h"

#include <iostream>
#include <fstream>

using namespace cmpl;

int main(int argc, char* argv[])
{
  Compiler c;
  InputParser input;

  try
  {
    input.parseArgs(argc, argv);
  }
  catch (ParameterError &e)
  {
    std::cerr << e.what();
    return 1;
  }
  
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
  
  if (input.cmdOptionExists("--lextest"))
  {
    const std::string &filename = input.getCmdOption("--lextest");
    if (!filename.empty())
    {
      std::ifstream file;
      
      file.open(filename);
      c.lextest(file);
      file.close();
    }
  }
  
  return 0;
}
