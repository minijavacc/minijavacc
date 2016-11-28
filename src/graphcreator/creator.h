#pragma once

#include "ast.h"
#include <libfirm/firm.h>
#include <stdexcept>
#include <iostream>

namespace cmpl
{
  
  class Creator
  {
  public:
    Creator(std::shared_ptr<Node> ast);
    ~Creator();
    
    void run();
    
    void dump();
    void createAssembler();
    int linkToRuntimeLibrary();
    
  private:
    std::shared_ptr<Node> ast;
  };
  
  class CreatorError : public std::runtime_error
  {
    public:
      CreatorError(const char* err) : std::runtime_error(err) { }
  };
  
}
