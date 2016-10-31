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
      void getAST(std::shared_ptr<Node> &t);
    
    private:
      Lexer& lexer;
      std::shared_ptr<Node> ast;
  };
  
  class SemanticError : public std::exception {};

}
