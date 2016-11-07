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
      std::unique_ptr<Node> parseProgram();
      std::unique_ptr<Node> parseClassDeclaration();
      std::unique_ptr<Node> parseClassMember();
      
      Lexer& lexer;
      std::unique_ptr<Node> ast;
      std::unique_ptr<Token> currentToken;
      
      inline void checkNextIsOSKTokenWithType(const TokenType& tokenType);
      template<typename T>
      inline void checkNextTokenTypeIs();
      inline void nextToken();
  };
  
  class SemanticError : public std::exception {};

}
