#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "stringtable.h"
#include "token.h"

#include <iostream>
#include <istream>
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
    std::cerr << sourcePreview(file, e.line, e.column) << "\n";
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
    std::cerr << sourcePreview(file, e.line, e.column) << "\n";
    return 1;
  }
  catch (ParserError &e) 
  {
    std::cerr << "parser error: " << e.what() << "\n";
    std::cerr << sourcePreview(file, e.line, e.column) << "\n";
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
    
    std::unique_ptr<Node> ast;
    parser.getAST(ast);
    
    PrettyPrinter printer(std::cout);
    ast->toString(printer);
  
    return 0;
  }
  catch (SyntaxError &e) 
  {
    std::cerr << "syntax error: " << e.what() << "\n";
    std::cerr << sourcePreview(file, e.line, e.column) << "\n";
    return 1;
  }
  catch (ParserError &e) 
  {
    std::cerr << "parser error: " << e.what() << "\n";
    std::cerr << sourcePreview(file, e.line, e.column) << "\n";
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
    
    std::unique_ptr<Node> ast;
    parser.getAST(ast);
    
    // TODO SemChecker semChecker(ast);
    // TODO semChecker.run();
    
    return 0;
  }
  catch (SyntaxError &e) 
  {
    std::cerr << "syntax error: " << e.what() << "\n";
    std::cerr << sourcePreview(file, e.line, e.column) << "\n";
    return 1;
  }
  catch (ParserError &e) 
  {
    std::cerr << "parser error: " << e.what() << "\n";
    std::cerr << sourcePreview(file, e.line, e.column) << "\n";
    return 1;
  }
}

std::string Compiler::sourcePreview(std::ifstream &file, unsigned int line, unsigned int column)
{
  std::string src;
  std::string ptr;
  
  // reset ifstream to start at beginning again
  file.clear();
  file.seekg(0, std::ios::beg);
  
  for (unsigned int i = 0; i < line; i++)
  {
    getline(file, src);
  }
  ptr = src;
  
  for (unsigned int i = 0; i < src.size(); i++)
  {
    // idea is to take the original string and keep characters like \t and \r
    // otherwise the position would be wrong
    if (i + 1 == column)
      ptr[i] = '^';
    else
      ptr[i] = ' ';
  }
  
  return src + "\n" + ptr;
}
