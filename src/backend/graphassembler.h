#pragma once

#include "amd64instructions.h"

#include <libfirm/firm.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <memory>

namespace cmpl
{

  class GraphAssembler
  {
  public:
    GraphAssembler(ir_graph* irg) : irg(irg) {}
    std::string run();
    
    std::vector<std::shared_ptr<Instruction>> irgSerialize();
    void irgRegisterAllocation(std::vector<std::shared_ptr<Instruction>> &instructions);
    std::string irgCodeGeneration(std::vector<std::shared_ptr<Instruction>> &instructions);
    
    regNum newReg();
    
  private:
    ir_graph* irg;
    
    regNum nextRegNum = 0;
  };
  
  class GraphAssemblerError : public std::runtime_error
  {
    public:
      GraphAssemblerError(const char* err) : std::runtime_error(err) { }
  };
}
