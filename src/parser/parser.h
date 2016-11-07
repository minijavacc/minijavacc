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
      
      inline void checkNextIsOSKTokenWithType(std::unique_ptr<Token>& token,
                                              const TokenType& tokenType);
      template<typename T>
      inline void checkNextTokenTypeIs(std::unique_ptr<Token>& token);
      inline void nextToken(std::unique_ptr<Token>& currentToken);
  };
  
  class SemanticError : public std::exception {};

}
