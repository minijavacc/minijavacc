#pragma once

#include "compiler.h"

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace cmpl
{
  class Option
  {
    public:
      Option(std::string string, std::function<int(std::ifstream &)> fn) :
        string(string), fn(fn) { }
      
      std::string string;
      std::function<int(std::ifstream &)> fn;
      std::string filename;
  };

  class InputParser
  {
    public:
      InputParser();
      void parseArgs(int &argc, char **argv);
      int handleArgs() const;
      
    private:
      // known parameters, their functions and whether they need an additional input file
      const std::vector<Option> knownOptions = {
        Option("--echo", Compiler::echo),
        Option("--lextest", Compiler::lextest),
        Option("--parsetest", Compiler::parsetest),
        Option("--print-ast", Compiler::printast),
        Option("--check", Compiler::semcheck)
      };

      std::vector<Option> givenOptions;
  };
  
  class ParameterError : public std::logic_error
  {
    public:
      ParameterError(const std::string& err) : std::logic_error(err) { }
  };

}
