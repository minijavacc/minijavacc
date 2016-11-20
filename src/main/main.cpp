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
    std::cerr << "Wrong Parameters: " << e.what();
    return 1;
  }

  try
  {
    return input.handleArgs();
  }
  catch (std::runtime_error &e)
  {
    std::cerr << "Exception occured: " << e.what();
    return 2;
  }
}
