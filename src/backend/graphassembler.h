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
    
    void irgSerialize();
    void irgRegisterAllocation();
    std::string irgCodeGeneration();
    
    regNum newReg();
    
  private:
    ir_graph* irg;
    std::shared_ptr<std::vector<std::shared_ptr<Instruction>>> instructions;
    size_t nargs;
    std::shared_ptr<subq_rsp> prolog_subq;

    regNum nextRegNum = 0;
  };
  
  class GraphAssemblerError : public std::runtime_error
  {
    public:
      GraphAssemblerError(const char* err) : std::runtime_error(err) { }
  };
}
