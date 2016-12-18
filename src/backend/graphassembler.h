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
  
  class LabeledBlock {
  public:
    Label label;
    shared_ptr<vector<shared_ptr<Instruction>>> instructions;
    shared_ptr<Instruction> exitInstruction;
    
    LabeledBlock() {
      instructions = make_shared<vector<shared_ptr<Instruction>>>();
    }
  };

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
    shared_ptr<map<Label, shared_ptr<LabeledBlock>>> blocks;
    std::shared_ptr<std::vector<Label>> labels; // topological order
    size_t nargs;
  };
  
  class GraphAssemblerError : public std::runtime_error
  {
    public:
      GraphAssemblerError(const char* err) : std::runtime_error(err) { }
  };
}
