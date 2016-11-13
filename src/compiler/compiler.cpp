#include "compiler.h"
#include "lexer.h"
#include "parser.h"
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
  try {
    Lexer l = Lexer();
    l.run(file);
  
    Parser p = Parser(l);
    p.run();
    
    std::unique_ptr<Node> n;
    p.getAST(n);
    
    PrettyPrinter r = PrettyPrinter(std::cout);
    n->toString(r);
  
    return 0;
  }
  catch (SyntaxError &e) 
  {
    std::cerr << "syntax error\n";
    return 1;
  }
  catch (SemanticError &e) 
  {
    std::cerr << "semantic error\n";
    return 1;
  }
}

void Compiler::output(std::string msg)
{
  std::cout << msg << "\n";
}

void Compiler::error(std::string msg)
{
  std::cerr << msg << "\n";
}
