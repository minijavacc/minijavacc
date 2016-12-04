#pragma once

#include "ast.h"
#include "checker.h"
#include <libfirm/firm.h>
#include <stdexcept>
#include <iostream>

namespace cmpl
{
  
  class Creator
  {
  public:
    Creator(Checker &checker);
    ~Creator();
    
    void run();
    
    void dumpGraph();
    void createBinary(std::string filename);
  private:
    std::shared_ptr<Node> ast;
    Checker &checker;
  };
  
  class CreatorError : public std::runtime_error
  {
    public:
      CreatorError(const char* err) : std::runtime_error(err) { }
  };
  
  class CreatorBackendError : public CreatorError
  {
    public:
      CreatorBackendError(const char* err) : CreatorError(err) { }
  };
  
}
