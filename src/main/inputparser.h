#pragma once

#include "../compiler/compiler.h"

#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace cmpl
{
  class Option
  {
    public:
      Option(std::function<int(std::ifstream &, std::string filename)> fn) :
        fn(fn) { }
      
      std::function<int(std::ifstream &, std::string filename)> fn;
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
      const std::map<std::string, Option> knownOptions = {
        {"--echo",         Option(Compiler::echo)},
        {"--lextest",      Option(Compiler::lextest)},
        {"--parsetest",    Option(Compiler::parsetest)},
        {"--print-ast",    Option(Compiler::printast)},
        {"--check",        Option(Compiler::semcheck)},
        {"--graph",        Option(Compiler::creategraph)},
        {"--compile-firm", Option(Compiler::compilefirm)},
        {"--compile",      Option(Compiler::compile)},
        {"--unittest",     Option(Compiler::unittest)}
      };
      
      std::vector<Option> givenOptions;
  };
  
  class ParameterError : public std::logic_error
  {
    public:
      ParameterError(const std::string& err) : std::logic_error(err) { }
  };

}
