#include "parser.h"

using namespace cmpl;

/************************ helper functions **************/

// if token array of lexer is empty (method returns false) throw semanticError for now,
// later depends on implementation of lexer running in parallel (to avoid polling)
inline void Parser::nextToken(std::unique_ptr<Token>& currentToken)
{
  if(!lexer.getNextToken(currentToken)) {
    throw SemanticError();
  }
}

// Checks whether given token is of specified type T
// returned pointer is for read access only as it's a raw pointer of unique_pointer
template<typename T>
inline void Parser::checkNextTokenTypeIs(std::unique_ptr<Token>& token)
{
  nextToken(token);
  if (!dynamic_cast<T*>(token.get()))
  {
    throw SemanticError();
  }
}

// combines checkTokenTypeIs with checking for specialized sub-type of operator/seperator tokens
inline void Parser::checkNextIsOSKTokenWithType(std::unique_ptr<Token>& token, const TokenType& tokenType)
{
  nextToken(token);
  OperatorSeperatorKeywordToken* osk_t;
  
  if (!(osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(token.get()))
      || osk_t->type != tokenType)
  {
    throw SemanticError();
  }
}

/************************ end helper functions **************/


void Parser::run()
{
  // each parseNONTERMINAL() function requests their token at the beginning
  ast = parseProgram();
}

std::unique_ptr<Node> Parser::parseProgram()
{
  std::unique_ptr<Token> currentToken;
  std::unique_ptr<Node> generatedNode;
  
  nextToken(currentToken);
  
  // check for class token
  
  generatedNode = parseClassDeclaration();
  
  return generatedNode;
}

std::unique_ptr<Node> Parser::parseClassDeclaration()
{
  std::unique_ptr<Token> currentToken;
  std::unique_ptr<Node> generatedNode;
  
  checkNextIsOSKTokenWithType(currentToken, T_K_CLASS);
  checkNextTokenTypeIs<IdentifierToken>(currentToken);
  checkNextIsOSKTokenWithType(currentToken, T_O_LBRACE);
  
  // call parseClassMember() and get AST
  generatedNode = parseClassMember();
  
  checkNextIsOSKTokenWithType(currentToken, T_O_RBRACE);
  
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
