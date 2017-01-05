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
    vector<ir_node *> phis;
    vector<shared_ptr<Instruction>> exitInstructions;
    
    LabeledBlock() {
      instructions = vector<shared_ptr<Instruction>>();
    }
    
    void finalize() {
      for (auto inst : exitInstructions) {
        instructions.push_back(inst);
      }
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
    void phiInsertion();
    
    // methods should be private, but must be called from irgNodeWalker()
    void insertProlog();
    void collectPhi(ir_node *node);
    void buildBlock(ir_node *node);
    void buildConst(ir_node *node);
    void buildCond(ir_node *node);
    void buildJmp(ir_node *node);
    void buildProj(ir_node *node);
    void buildAdd(ir_node *node);
    void buildSub(ir_node *node);
    void buildDiv(ir_node *node);
    void buildMul(ir_node *node);
    void buildMinus(ir_node *node);
    void buildMod(ir_node *node);
    void buildReturn(ir_node *node);
    
  private:
    ir_graph* irg;
    size_t nargs;
    
    map<Label, shared_ptr<LabeledBlock>> blocks;
    vector<Label> labels; // topological order 
    map<long, long> registers;
    map<long, long> helperRegisters;
    map<long, Label> nodeNrToLabel;
    long nextFreeRegister = 0;
    long nextFreeLabel = 0;
    string labelPrefix;
    
    long getRegister(ir_node *node);
    long getHelperRegister(ir_node *node);
    Label getLabel(ir_node *node);

    void allocI2to1(shared_ptr<Instruction> instr, I2to1 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI2to0(shared_ptr<Instruction> instr, I2to0 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI1to1(shared_ptr<Instruction> instr, I1to1 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI1to0(shared_ptr<Instruction> instr, I1to0 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocMoveFromStack(shared_ptr<Instruction> instr, movl_from_stack *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocMoveFromImm(shared_ptr<Instruction> instr, movl_from_imm *i, vector<shared_ptr<Instruction>> &instructions_);
  };
  
  class GraphAssemblerError : public std::runtime_error
  {
    public:
      GraphAssemblerError(const char* err) : std::runtime_error(err) { }
  };
}
