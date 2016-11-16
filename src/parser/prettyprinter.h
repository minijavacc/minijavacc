#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace cmpl
{
  class PrettyPrinter
  {
    private:
      const std::string indent = "   ";
      std::ostream& printStream;
      std::string indents;
      bool indentsPrinted;
    public:
      PrettyPrinter(std::ostream& printStream) : printStream(printStream), indentsPrinted(false) { };
      void print(const std::string &s);
      void println(const std::string &s);
      void addIndent();
      void removeIndent();
      
      std::ostream& getStream();
  };
}
