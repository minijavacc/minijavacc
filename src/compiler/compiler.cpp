#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "stringtable.h"
#include "token.h"
#include "checker.h"
#include "prettyprinter.h"

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
    Lexer lexer;
    lexer.run(file);
  
    std::unique_ptr<Token> token;
    while (lexer.getNextToken(token))
    {
      std::cout << token->getStringValue() << "\n";
    }
  
    std::cout << "EOF" << "\n";
  
    return 0;
  }
  catch (SyntaxError &e) 
  {
    std::cerr << "syntax error: " << e.what() << "\n";
    return 1;
  }
}

int Compiler::parsetest(std::ifstream &file)
{
  try {
    Lexer lexer;
    lexer.run(file);
  
    Parser parser(lexer);
    parser.run();
  
    return 0;
  }
  catch (SyntaxError &e) 
  {
    std::cerr << "syntax error: " << e.what() << "\n";
    return 1;
  }
  catch (SemanticError &e) 
  {
    std::cerr << "semantic error: " << e.what() << "\n";
    return 1;
  }
}

int Compiler::printast(std::ifstream &file)
{
  try {
    Lexer lexer;
    lexer.run(file);
  
    Parser parser(lexer);
    parser.run();
    
    std::shared_ptr<Node> ast = parser.getAST();
    
    std::shared_ptr<PrettyPrinter> printer(new PrettyPrinter(std::cout));
    ast->accept(printer);
  
    return 0;
  }
  catch (SyntaxError &e) 
  {
    std::cerr << "syntax error: " << e.what() << "\n";
    return 1;
  }
  catch (SemanticError &e) 
  {
    std::cerr << "semantic error: " << e.what() << "\n";
    return 1;
  }
}

int Compiler::semcheck(std::ifstream &file)
{
  try {
    Lexer lexer;
    lexer.run(file);
  
    Parser parser(lexer);
    parser.run();
    
    Checker checker(parser);
    checker.run();
    
    return 0;
  }
  catch (SyntaxError &e) 
  {
    std::cerr << "syntax error: " << e.what() << "\n";
    return 1;
  }
  catch (SemanticError &e) 
  {
    std::cerr << "semantic error: " << e.what() << "\n";
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
