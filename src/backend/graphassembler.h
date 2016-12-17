#pragma once

#include "amd64instructions.h"

#include <libfirm/firm.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <memory>
#include <map>

namespace cmpl
{
  using namespace std;

  class GraphAssembler
  {
  public:
    GraphAssembler(ir_graph* irg) : irg(irg) {}
    std::string run();
    
    void irgSerialize();
    void irgRegisterAllocation();
    std::string irgCodeGeneration();
    
  private:
    ir_graph* irg;
    shared_ptr<map<Label, shared_ptr<vector<shared_ptr<Instruction>>>>> blocks;
    std::shared_ptr<std::vector<Label>> labels; // topological order
    size_t nargs;
  };
  
  class GraphAssemblerError : public std::runtime_error
  {
    public:
      GraphAssemblerError(const char* err) : std::runtime_error(err) { }
  };
}
