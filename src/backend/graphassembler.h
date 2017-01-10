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
    vector<shared_ptr<Instruction>> instructions;
    shared_ptr<Instruction> exitInstruction;
    
    LabeledBlock() {
      instructions = vector<shared_ptr<Instruction>>();
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
    void buildCall(ir_node *node);
    
  private:
    ir_graph* irg;
    size_t nargs;
    
    map<Label, shared_ptr<LabeledBlock>> blocks;
    vector<Label> labels; // topological order 
    map<long, long> registers;
    map<long, Label> nodeNrToLabel;
    long nextFreeRegister = 0;
    long nextFreeLabel = 0;
    string labelPrefix;
    
    long allocateReg(ir_node *node);
    Label getLabel(ir_node *node);
    shared_ptr<LabeledBlock> getCurrentBlock();

    void allocI2to1(shared_ptr<Instruction> instr, I2to1 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI1to1(shared_ptr<Instruction> instr, I1to1 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI1to0(shared_ptr<Instruction> instr, I1to0 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocMoveFromStack(shared_ptr<Instruction> instr, movl_from_stack *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocMoveFromImm(shared_ptr<Instruction> instr, movl_from_imm *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocCall(shared_ptr<Instruction> instr, movl_from_imm *i, vector<shared_ptr<Instruction>> &instructions_);
  };
  
  class GraphAssemblerError : public std::runtime_error
  {
    public:
      GraphAssemblerError(const char* err) : std::runtime_error(err) { }
  };
}
