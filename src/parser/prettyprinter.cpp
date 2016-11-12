#include "prettyprinter.h"

using namespace cmpl;

void PrettyPrinter::print(const std::string &s)
{
  *printStream << indents << s;
}

void PrettyPrinter::println(const std::string &s)
{
  *printStream << indents << s << '\n';
}

void PrettyPrinter::addIndent()
{
  indents.append(indent);
}

void PrettyPrinter::removeIndent()
{
  indents.erase(indents.begin(),indents.end()-indent.size());
}

std::unique_ptr<std::ostream> PrettyPrinter::getStream()
{
  return std::move(printStream);
}

