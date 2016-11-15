#pragma once

#include "lexer.h"
#include "node.h"

#include <memory>
#include <stdexcept>

namespace cmpl
{

  class Parser
  {
    public:
      Parser(Lexer &lexer) : lexer(lexer) { };
      void run();
      void getAST(std::unique_ptr<Node> &n);
    
    private:
      /*
       * All parsing functions EXCEPT parseClassDeclaration() and parseProgram() have currentToken point to their first
       * token upon calling and point to the next token on returning.
       * parseProgram() starts with no tokens and returns when there are no tokens left.
       * parseClassDeclaration() returns with the last symbol ('}') as currentToken.
       */
      std::unique_ptr<Program>          parseProgram();
      std::unique_ptr<ClassDeclaration> parseClassDeclaration();
      std::unique_ptr<ClassMember>      parseClassMember();
      std::unique_ptr<Type>             parseType();
      std::unique_ptr<BasicType>        parseBasicType();
      std::unique_ptr<Parameter>        parseParameter();
      std::unique_ptr<BlockStatement>   parseBlockStatement();
      std::unique_ptr<BlockStatement>   parseLocalVarDecl();
      std::unique_ptr<Block>            parseBlock();
      std::unique_ptr<Statement>        parseStatement();
      std::unique_ptr<Statement>        parseIfElseStatement();
      std::unique_ptr<Statement>        parseWhileStatement();
      std::unique_ptr<Statement>        parseReturnStatement();
      std::unique_ptr<Statement>        parseExpressionStatement();
      std::unique_ptr<Expression>       parseExpression(unsigned int minPrecedence = 0);
      std::unique_ptr<Expression>       parseUnaryExpression();
      std::unique_ptr<Expression>       parsePostfixExpression();
      std::unique_ptr<Expression>       parsePrimaryExpression();
      
      Lexer& lexer;
      std::unique_ptr<Node> ast;
      std::unique_ptr<Token> currentToken;

      inline void error(const std::string &err);
      
      inline void nextToken();
      
      inline StringIdentifier getIdentifierFromCurrent();
      inline StringIdentifier getIdentifierFromNext();
      
      template<typename T>
      inline void assureCurrentTokenTypeIs();
      template<typename T>
      inline void assureNextTokenTypeIs();
      inline void assureCurrentIsOSKTokenWithType(const TokenType& tokenType);
      inline void assureNextIsOSKTokenWithType(const TokenType& tokenType);

      template<typename T>
      inline bool isCurrentTokenOfType();
      template<typename T>
      inline bool isNextTokenOfType();
      inline bool isCurrentTokenOSKTokenOfType(const TokenType& tokenType);
      inline bool isCurrentTokenOSKTokenOfCategory(const TokenCategory& tokenCategory, TokenType& tokenTypeOut);
      inline bool isNextTokenOSKTokenOfType(const TokenType& tokenType);

  };
  
  class SemanticError : public std::runtime_error
  {
    public:
      SemanticError(const std::string& err) : std::runtime_error(err) { }
  };

}
