#pragma once

#include "../structures/ast.h"
#include "../semantics/checker.h"
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
    
    void dumpGraphs(std::string suffix = "");
    void createBinary(std::string filename, bool generateDebugInformation = false);
  private:
    std::shared_ptr<Node> ast;
    Checker &checker;
    
    ir_mode *mode_P64;
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
