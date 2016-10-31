#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace cmpl
{

  class InputParser
  {
    typedef std::unordered_map<std::string,bool> parametermap;
    typedef std::unordered_map<std::string,std::string> parameters;

    public:
      InputParser();
      void parseArgs(int &argc, char **argv);
      const std::string& getCmdOption(const std::string &option) const;
      bool cmdOptionExists(const std::string &option) const;
      
    private:
      // known parameters and whether they need an additional value (e.g. an input file)
      const parametermap knownParameters = {{"--echo", true}, {"--lextest", true}};

      parameters givenParameters;
  };
  
  class ParameterError : public std::invalid_argument
  {
    public:
      ParameterError(std::string& err) : std::invalid_argument(err) { }
  };

}
