#include "compiler.h"
#include "inputparser.h"

#include <iostream>

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
  
  return input.handleArgs();
}
