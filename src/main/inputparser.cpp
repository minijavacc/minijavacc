#include "inputparser.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace cmpl;

InputParser::InputParser() { }

void InputParser::parseArgs(int &argc, char **argv)
{
  for (int i=1; i < argc; ++i) {
    std::string argument = std::string(argv[i]);
    auto it = knownOptions.find(argument);
    
    if(it != knownOptions.end()) {
      if (++i < argc) {
        Option opt(it->second);
        opt.filename = std::string(argv[i]);
        givenOptions.push_back(opt);
      } else {
        std::string err("insufficient arguments\n");
        throw ParameterError(err);
      }
    } else {
      // try generic compile
      Option opt(Compiler::compile);
      opt.filename = argument;
      givenOptions.push_back(opt);
    }
  }
}

int InputParser::handleArgs() const
{
  for (auto opt : givenOptions)
  {
    std::ifstream file(opt.filename);
    
    if (file.bad())
    {
      std::string err = "Can't read input file " + opt.filename;
      throw std::runtime_error(err);
    }

    // actual call
    return opt.fn(file, opt.filename);
  }
  
  return 0;
}
