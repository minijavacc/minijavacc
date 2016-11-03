#include "parser.h"

using namespace cmpl;

void Parser::run()
{
  // each parseNONTERMINAL() function requests their token at the beginning
  ast = parseProgram();
}

std::unique_ptr<Node> Parser::parseProgram()
{
  std::unique_ptr<Token> currentToken;
  std::unique_ptr<Node> generatedNode;
  
  // TODO: do something if array is empty (returns false) -> just throw semanticError?
  lexer.getNextToken(currentToken);
  
  generatedNode = parseClassDeclaration();
  
  return generatedNode;
}

std::unique_ptr<Node> Parser::parseClassDeclaration()
{
  std::unique_ptr<Token> currentToken;
  std::unique_ptr<Node> generatedNode;
  
  // TODO: do something if array is empty (returns false) -> just throw semanticError?
  lexer.getNextToken(currentToken);
  
  // check if current token is an OperatorSeperatorKeywordToken
  OperatorSeperatorKeywordToken* osk_t; // only for read access! raw pointer of unique_pointer
  if (!(osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get())))
  {
    throw SemanticError();
  }
  
  // check if keyword is 'class'
  if (osk_t->type != T_K_CLASS)
  {
    throw SemanticError();
  }
  
  // TODO: do something if array is empty (returns false) -> just throw semanticError?
  lexer.getNextToken(currentToken);
  
  // check if current token is an IdentifierToken
  IdentifierToken* i_t; // only for read access! raw pointer of unique_pointer
  if (!(i_t = dynamic_cast<IdentifierToken*>(currentToken.get())))
  {
    throw SemanticError();
  }
  
  // TODO: do something if array is empty (returns false) -> just throw semanticError?
  lexer.getNextToken(currentToken);
  
  // check if current token is an OperatorSeperatorKeywordToken
  // reuse old osk_t pointer
  if (!(osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get())))
  {
    throw SemanticError();
  }
  
  // check if token is '{'
  if (osk_t->type != T_O_LBRACE)
  {
    throw SemanticError();
  }
  
  
  
  
  // call parseClassMember() and get AST
  generatedNode = parseClassMember();
  
  
  
  
  // TODO: do something if array is empty (returns false) -> just throw semanticError?
  lexer.getNextToken(currentToken);
  
  // check if current token is an OperatorSeperatorKeywordToken
  // reuse old osk_t pointer
  if (!(osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get())))
  {
    throw SemanticError();
  }
  
  // check if token is '}'
  if (osk_t->type != T_O_RBRACE)
  {
    throw SemanticError();
  }
  
  return generatedNode;
}

std::unique_ptr<Node> Parser::parseClassMember()
{
  return nullptr;
}

void Parser::getAST(std::unique_ptr<Node> &n)
{
  n = std::move(ast);
}
