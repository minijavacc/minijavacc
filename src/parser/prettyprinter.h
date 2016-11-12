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
      std::unique_ptr<std::ostream> printStream;
      std::string indents;
    public:
      PrettyPrinter(std::unique_ptr<std::ostream> &printStream) : printStream(std::move(printStream)) { };
      void print(const std::string &s);
      void println(const std::string &s);
      void addIndent();
      void removeIndent();
      
      std::unique_ptr<std::ostream> getStream();
  };
}
