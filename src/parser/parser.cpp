#include "parser.h"

using namespace cmpl;


/************************ helper functions **************/


// if token array of lexer is empty (method returns false) throw semanticError for now,
// later depends on implementation of lexer running in parallel (to avoid polling)
inline void Parser::nextToken()
{
  if(!lexer.getNextToken(currentToken)) {
    throw SemanticError();
  }
}

// Checks whether next token is of specified type T, trows exception in case that not
template<typename T>
inline void Parser::assureCurrentTokenTypeIs()
{
  if (!isCurrentTokenOfType<T>())
  {
    throw SemanticError();
  }
}

// Checks whether next token is of specified type T, trows exception in case that not
template<typename T>
inline void Parser::assureNextTokenTypeIs()
{
  nextToken();
  assureCurrentTokenTypeIs<T>();
}

// combines assureNextTokenTypeIs with checking for specialized sub-type of operator/seperator tokens
inline void Parser::assureCurrentIsOSKTokenWithType(const TokenType& tokenType)
{
  if (!isCurrentTokenOSKTokenOfType(tokenType))
  {
    throw SemanticError();
  }
}

// combines assureNextTokenTypeIs with checking for specialized sub-type of operator/seperator tokens
inline void Parser::assureNextIsOSKTokenWithType(const TokenType& tokenType)
{
  nextToken();
  assureCurrentIsOSKTokenWithType(tokenType);
}


// Checks whether current token is of specified type T
template<typename T>
inline bool Parser::isCurrentTokenOfType() {
  return dynamic_cast<T*>(currentToken.get());
}

// Checks whether next token is of specified type T
template<typename T>
inline bool Parser::isNextTokenOfType() {
  nextToken();
  return isCurrentTokenOfType<T>();
}

// combines isCurrentTokenOfType with checking for specialized sub-type of operator/seperator tokens
inline bool Parser::isCurrentTokenOSKTokenOfType(const TokenType& tokenType) {
  OperatorSeperatorKeywordToken* osk_t;
  return ((osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get()))
          && (osk_t->type != tokenType));
}

// combines isNextTokenOfType with checking for specialized sub-type of operator/seperator tokens
inline bool Parser::isNextTokenOSKTokenOfType(const TokenType& tokenType) {
  nextToken();
  return isCurrentTokenOSKTokenOfType(tokenType);
}


/************************ end helper functions **************/


void Parser::run()
{
  // each parseNONTERMINAL() function requests their token at the beginning
  ast = parseProgram();
}

std::unique_ptr<Node> Parser::parseProgram()
{
  std::vector<std::unique_ptr<Node>> generatedNodes;
  
  // multile classes
  while(lexer.hasNextToken()) {
    assureNextIsOSKTokenWithType(T_K_CLASS);
    generatedNodes.push_back(parseClassDeclaration());
  }
  
  std::unique_ptr<ProgramNode> generatedNode;//(generatedNodes);
  return generatedNode;
}

/* start: current = "class" */
std::unique_ptr<Node> Parser::parseClassDeclaration()
{
  std::vector<std::unique_ptr<Node>> generatedNodes;
  
  assureNextTokenTypeIs<IdentifierToken>();
  assureNextIsOSKTokenWithType(T_O_LBRACE);
  
  // multile class members
  nextToken();
  while(!isCurrentTokenOSKTokenOfType(T_O_RBRACE)) {
    generatedNodes.push_back(parseClassMember());
    //TODO depending on what is returned maybe use current
    nextToken();
  }
  
  std::unique_ptr<ClassNode> generatedNode;//(generatedNodes);
  return generatedNode;
}

std::unique_ptr<Node> Parser::parseClassMember()
{
  std::vector<std::unique_ptr<Node>> generatedNodes;
  
  assureNextIsOSKTokenWithType(T_K_PUBLIC);
  
  if(isNextTokenOSKTokenOfType(T_K_STATIC)) {
    // generatedNodes.push_back(parseMainMethod());
  } else {
    generatedNodes.push_back(parseType());
    assureCurrentTokenTypeIs<IdentifierToken>();
    if(isNextTokenOSKTokenOfType(T_O_SEMICOLON)) {
      // Field
    } else if(isCurrentTokenOSKTokenOfType(T_O_LPAREN)) {
      // Method
    } else {
      throw SemanticError();
    }
  }
  
  std::unique_ptr<ClassMemberNode> generatedNode;//(generatedNodes);
  return generatedNode;
}

std::unique_ptr<Node> Parser::parseType()
{
  std::vector<std::unique_ptr<Node>> generatedNodes;
  std::string type;
  // basic type
  if(isCurrentTokenOSKTokenOfType(T_K_BOOLEAN)) {
    type="boolean";
  } else if(isCurrentTokenOfType<IntegerLiteralToken>()) {
    type="int";
  } else if(isCurrentTokenOSKTokenOfType(T_K_VOID)) {
    type="void";
  } else if(assureCurrentTokenTypeIs<IdentifierToken>()) {
    type=(dynamic_cast<IdentifierToken>currentToken);
  }
  
  while(isCurrentTokenOSKTokenOfType(T_O_LBRACK)) {
    assureNextIsOSKTokenWithType(T_O_RBRACK);
    
  }
  
  std::unique_ptr<TypeNode> generatedNode;//(generatedNodes);
  return generatedNode;
}

std::unique_ptr<Node> Parser::parseExpression()
{
  return nullptr;
}


void Parser::getAST(std::unique_ptr<Node> &n)
{
  n = std::move(ast);
}
