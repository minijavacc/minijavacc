#pragma once

#include "lexer.h"
#include "node.h"

#include <memory>

namespace cmpl
{

  class Parser
  {
    public:
      Parser(Lexer &lexer) : lexer(lexer) {};
      void run();
      void getAST(std::unique_ptr<Node> &n);
    
    private:
      /* all parsing functions have currentToken point to the position one after their last token */
      std::unique_ptr<Node> parseProgram();
      std::unique_ptr<Node> parseClassDeclaration();
      std::unique_ptr<Node> parseClassMember();
      std::unique_ptr<Node> parseType();
      std::unique_ptr<Node> parseBasicType();
      
      Lexer& lexer;
      std::unique_ptr<Node> ast;
      std::unique_ptr<Token> currentToken;

      inline void nextToken();
      
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
      inline bool isNextTokenOSKTokenOfType(const TokenType& tokenType);

  };
  
  class SemanticError : public std::exception {};

}
