#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "stringtable.h"
#include "token.h"
#include "checker.h"
#include "prettyprinter.h"
#include "creator.h"
#include "backend.h"
#include "optimizer.h"

#include <iostream>
#include <istream>
#include <fstream>
#include <string>

using namespace cmpl;

int Compiler::echo(std::ifstream &file, std::string filename)
{
  std::string line;
  
  while(std::getline(file, line))
  {
    std::cout << line << "\n";
  }
  
  return 0;
}

int Compiler::lextest(std::ifstream &file, std::string filename)
{
  try {
    Lexer lexer(file);
    lexer.run();
  
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

int Compiler::parsetest(std::ifstream &file, std::string filename)
{
  try {
    Lexer lexer(file);
    lexer.run();
  
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

int Compiler::printast(std::ifstream &file, std::string filename)
{
  try {
    Lexer lexer(file);
    lexer.run();
  
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

int Compiler::semcheck(std::ifstream &file, std::string filename)
{
  try {
    Lexer lexer(file);
    lexer.run();
  
    Parser parser(lexer);
    parser.run();
    
    Checker checker(parser);
    checker.run();
    
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
  catch (SemanticError &e)
  {
    std::cerr << "semantic error: " << e.what() << "\n";
    //std::cerr << sourcePreview(file, e.line, e.column) << "\n"; TODO: make semantic errors useful
    return 1;
  }
}

int Compiler::creategraph(std::ifstream &file, std::string filename)
{
  try {
    Lexer lexer(file);
    lexer.run();
  
    Parser parser(lexer);
    parser.run();
    
    Checker checker(parser);
    checker.run();
    
    Creator creator(checker);
    creator.run();
    
    Optimizer optimizer(creator);
    optimizer.run();
    
    creator.dumpGraphs();
    std::cout << "dumped graph files *.vcg\n";
    
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
  catch (SemanticError &e)
  {
    std::cerr << "semantic error: " << e.what() << "\n";
    //std::cerr << sourcePreview(file, e.line, e.column) << "\n"; TODO: make semantic errors useful
    return 1;
  }
  catch (CreatorError &e) 
  {
    std::cerr << "creator error: " << e.what() << "\n";
    return 1;
  }
}

int Compiler::compilefirm(std::ifstream &file, std::string filename)
{
  try {
    Lexer lexer(file);
    lexer.run();
  
    Parser parser(lexer);
    parser.run();
    
    Checker checker(parser);
    checker.run();
    
    Creator creator(checker);
    creator.run();
    
    Optimizer optimizer(creator);
    optimizer.run();
    
    creator.createBinary(filename);
    
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
  catch (SemanticError &e)
  {
    std::cerr << "semantic error: " << e.what() << "\n";
    //std::cerr << sourcePreview(file, e.line, e.column) << "\n"; TODO: make semantic errors useful
    return 1;
  }
  catch (CreatorError &e) 
  {
    std::cerr << "creator error: " << e.what() << "\n";
    return 1;
  }
}

int Compiler::compile(std::ifstream &file, std::string filename)
{
  try {
    Lexer lexer(file);
    lexer.run();
  
    Parser parser(lexer);
    parser.run();
    
    Checker checker(parser);
    checker.run();
    
    Creator creator(checker);
    creator.run();
    
    Optimizer optimizer(creator);
    optimizer.run();
    
    Backend backend(creator);
    backend.run(filename);
    
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
  catch (SemanticError &e)
  {
    std::cerr << "semantic error: " << e.what() << "\n";
    //std::cerr << sourcePreview(file, e.line, e.column) << "\n"; TODO: make semantic errors useful
    return 1;
  }
  catch (CreatorError &e) 
  {
    std::cerr << "creator error: " << e.what() << "\n";
    return 1;
  }
  catch (BackendError &e) 
  {
    std::cerr << "backend error: " << e.what() << "\n";
    return 1;
  }
}

std::string Compiler::sourcePreview(std::ifstream &file, unsigned int line, unsigned int column)
{
  std::string src;
  
  // reset ifstream to start at beginning again
  file.clear();
  file.seekg(0, std::ios::beg);
  
  for (unsigned int i = 0; i < line; i++)
  {
    getline(file, src);
  }
  
  /* TODO: implement marker that points to column X
   * be careful with \t and \r!
  for (unsigned int i = 0; i < src.size(); i++)
  {
    // idea is to take the original string and keep characters like \t and \r
    // otherwise the position would be wrong
    if (i + 1 == column)
      ptr[i] = '^';
    else if (ptr[i] > 31 && ptr[i] < 127)
      ptr[i] = ' ';
  }
  */
  return std::to_string(line) + ":" + std::to_string(column) + ": " + src;
}
