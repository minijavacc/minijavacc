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
      std::shared_ptr<Node> getAST();
    
    private:
      /*
       * All parsing functions EXCEPT parseClassDeclaration() and parseProgram() have currentToken point to their first
       * token upon calling and point to the next token on returning.
       * parseProgram() starts with no tokens and returns when there are no tokens left.
       * parseClassDeclaration() returns with the last symbol ('}') as currentToken.
       */
      std::shared_ptr<Program>          parseProgram();
      std::shared_ptr<ClassDeclaration> parseClassDeclaration();
      std::shared_ptr<ClassMember>      parseClassMember();
      std::shared_ptr<Type>             parseType();
      std::shared_ptr<BasicType>        parseBasicType();
      std::shared_ptr<Parameter>        parseParameter();
      std::shared_ptr<BlockStatement>   parseBlockStatement();
      std::shared_ptr<BlockStatement>   parseLocalVarDecl();
      std::shared_ptr<Block>            parseBlock();
      std::shared_ptr<Statement>        parseStatement();
      std::shared_ptr<Statement>        parseIfElseStatement();
      std::shared_ptr<Statement>        parseWhileStatement();
      std::shared_ptr<Statement>        parseReturnStatement();
      std::shared_ptr<Statement>        parseExpressionStatement();
      std::shared_ptr<Expression>       parseExpression(unsigned int minPrecedence = 0);
      std::shared_ptr<Expression>       parseUnaryExpression();
      std::shared_ptr<Expression>       parsePostfixExpression();
      std::shared_ptr<Expression>       parsePrimaryExpression();
      
      Lexer& lexer;
      std::shared_ptr<Node> ast;
      std::unique_ptr<Token> currentToken;
      
      inline void nextToken();
      
      void error(const std::string &err);
      
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
  
  class ParserError : public std::runtime_error
  {
    public:
      ParserError(const char* err, unsigned int line, unsigned int column)
        : std::runtime_error(err), line(line), column(column) { }
      unsigned int line;
      unsigned int column;
  };

}
