#include "prettyprinter.h"

using namespace cmpl;

void PrettyPrinter::print(const std::string &s)
{
  if(indentsPrinted) {
    printStream << s;
  } else {
    printStream << indents << s;
    indentsPrinted = true;
  }
}

void PrettyPrinter::println(const std::string &s)
{
  printStream << indents << s << '\n';
  indentsPrinted = false;
}

void PrettyPrinter::addIndent()
{
  indents.append(indent);
}

void PrettyPrinter::removeIndent()
{
  indents.erase(indents.begin(),indents.end()-indent.size());
}

std::ostream& PrettyPrinter::getStream()
{
  return printStream;
}

