#include "compiler.h"
#include "inputparser.h"

#include <iostream>
#include <functional>
#include <fstream>
#include <stdexcept>

using namespace cmpl;


void readFile(const std::string& filename, std::ifstream& file)
{
  file.open(filename);
  if (file.bad())
  {
    std::string err = "Can't read input file " + filename;
    throw std::runtime_error(err);
  }
}

void printException(const std::exception e)
{
  std::cerr << e.what();
}

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
    std::ifstream file;
    try
    {
      readFile(filename, file);
      c.echo(file);
    }
    catch (std::runtime_error &err)
    {
      printException(err);
      return 2;
    }
    
  }
  
  if (input.cmdOptionExists("--lextest"))
  {
    const std::string &filename = input.getCmdOption("--lextest");
    std::ifstream file;
    try
    {
      readFile(filename, file);
    }
    catch (std::runtime_error &err)
    {
      printException(err);
      return 3;
    }
    c.lextest(file);
  }
  
  return 0;
}
