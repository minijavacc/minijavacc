#include "compiler.h"
#include "lexer.h"
#include "stringtable.h"
#include "token.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace cmpl;

int Compiler::echo(std::ifstream &file)
{
  std::string line;
  
  while(std::getline(file, line))
  {
    std::cout << line << "\n";
  }
  
  return 0;
}

int Compiler::lextest(std::ifstream &file)
{
  try {
    Lexer l = Lexer();
    l.run(file);
  
    std::unique_ptr<Token> t;
    while (l.getNextToken(t))
    {
      std::cout << t->getStringValue() << "\n";
    }
  
    std::cout << "EOF" << "\n";
  
    return 0;
  }
  catch (SyntaxError &e) 
  {
    std::cerr << "error\n";
    return 1;
  }
}

int Compiler::parsetest(std::ifstream &file)
{
  return 0;
}

void Compiler::output(std::string msg)
{
  std::cout << msg << "\n";
}

void Compiler::error(std::string msg)
{
  std::cerr << msg << "\n";
}
