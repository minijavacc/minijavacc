#include "inputparser.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace cmpl;

InputParser::InputParser() { }

void InputParser::parseArgs(int &argc, char **argv)
{
  for (int i=1; i < argc; ++i)
  {
    bool paramFound = false;

    for (Option opt : knownOptions)
    {
      if (opt.string.compare(std::string(argv[i])) == 0) // option known
      {
          if (++i < argc)
          {
            opt.filename = std::string(argv[i]);
            givenOptions.push_back(opt);
          }
          else
          {
            std::string err("insufficient arguments\n");
            throw ParameterError(err);
          }
        paramFound = true;
        break;
      }
    }
    
    if (!paramFound)
    {
      std::string err("invalid parameters\n");
      throw ParameterError(err);
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
    return opt.fn(file);
  }
}
