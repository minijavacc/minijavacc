#include "inputparser.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using namespace cmpl;

InputParser::InputParser() { }

void InputParser::parseArgs(int &argc, char **argv)
{
  for (int i=1; i < argc; ++i)
  {
    auto parameter = std::string(argv[i]);
    auto it = this->knownParameters.find(parameter);
    if (it != this->knownParameters.end()) // parameter known
    {
      std::string value = "";
      if ((*it).second) // parameter needs a value
      {
        value = std::string(argv[++i]);
      }
      this->givenParameters.emplace(parameter, value);
    }
    else
    {
      std::string err("invalid argument\n");
      throw ParameterError(err);
    }
  }
}

const std::string& InputParser::getCmdOption(const std::string &option) const
{
  auto it = this->givenParameters.find(option);
  return (*it).second;
}

bool InputParser::cmdOptionExists(const std::string &option) const
{
  auto it = this->givenParameters.find(option);
  return it != this->givenParameters.end();
}
