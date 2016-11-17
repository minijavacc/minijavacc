#include "compiler.h"
#include "inputparser.h"

#include <iostream>

using namespace cmpl;

void printException(const std::exception e)
{
  std::cerr << "Exception occured: " << e.what();
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
    printException(e);
    return 1;
  }

  try
  {
    return input.handleArgs();
  }
  catch (std::runtime_error &e)
  {
    printException(e);
    return 2;
  }
}
