#pragma once

#include "lexer.h"
#include "node.h"

#include <memory>

namespace cmpl
{

  class Parser
  {
    public:
      Parser(Lexer &lexer) : lexer(lexer) { };
      void run();
      void getAST(std::unique_ptr<Node> &n);
    
    private:
      /* all parsing functions have currentToken point to their first token upon calling
         and point to the next token on returning. */
      std::unique_ptr<Program> parseProgram();
      std::unique_ptr<ClassDeclaration> parseClassDeclaration();
      std::unique_ptr<ClassMember> parseClassMember();
      std::unique_ptr<Type> parseType();
      std::unique_ptr<BasicType> parseBasicType();
      std::unique_ptr<Parameter> parseParameter();
      std::unique_ptr<Block> parseBlock();
      std::unique_ptr<Statement> parseStatement();
      std::unique_ptr<Statement> parseIfElseStatement();
      std::unique_ptr<Statement> parseWhileStatement();
      std::unique_ptr<Statement> parseReturnStatement();
      std::unique_ptr<Expression> parseExpression(unsigned int minPrecedence = 0);
      std::unique_ptr<UnaryExpression> parseUnaryExpression();
      
      Lexer& lexer;
      std::unique_ptr<Node> ast;
      std::unique_ptr<Token> currentToken;

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
  
  class SemanticError : public std::exception { };

}
