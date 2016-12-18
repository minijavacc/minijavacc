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
    
    // methods should be private, but must be called from irgNodeWalker()
    void insertProlog();
    void handlePhi(ir_node *node);
    void buildBlock(ir_node *node);
    void buildConst(ir_node *node);
    void buildCond(ir_node *node);
    void buildJmp(ir_node *node);
    void buildProj(ir_node *node);
    void buildAdd(ir_node *node);
    void buildReturn(ir_node *node);
    
  private:
    ir_graph* irg;
    size_t nargs;
    
    shared_ptr<map<Label, shared_ptr<LabeledBlock>>> blocks;
    shared_ptr<vector<Label>> labels; // topological order
    map<long, long> registers;
    map<long, Label> nodeNrToLabel;
    long nextFreeRegister = 0;
    long nextFreeLabel = 0;
    string labelPrefix;
    
    long allocateReg(ir_node *node);
    Label getLabel(ir_node *node);
    shared_ptr<LabeledBlock> getCurrentBlock();

    void allocI2to1();
  };
  
  class GraphAssemblerError : public std::runtime_error
  {
    public:
      GraphAssemblerError(const char* err) : std::runtime_error(err) { }
  };
}
