#pragma once

#include "amd64instructions.h"
#include "assemblercontext.h"

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
  
  class RegisterAllocator
  {
	public:
    RegisterAllocator(shared_ptr<map<Label, shared_ptr<LabeledBlock>>> blocks, shared_ptr<vector<Label>> labels, shared_ptr<StackFrameAllocation> sfa) : blocks(blocks), labels(labels), stackFrameAllocation(sfa) {};
    
    void run();
    
    
    // Only interface to graphassembler
    shared_ptr<map<Label, shared_ptr<LabeledBlock>>> blocks;
    shared_ptr<vector<Label>> labels; // topological order
    shared_ptr<StackFrameAllocation> stackFrameAllocation;
   
//    void deliverValue(shared_ptr<Value> &src, shared_ptr<Value> &dest, vector<shared_ptr<Instruction>> &instructions_);
    void allocABtoB(shared_ptr<Instruction> instr, ABtoB *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocABto_(shared_ptr<Instruction> instr, ABto_ *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocAtoA(shared_ptr<Instruction> instr, AtoA *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocAtoB(shared_ptr<Instruction> instr, AtoB *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocAto_(shared_ptr<Instruction> instr, Ato_ *i, vector<shared_ptr<Instruction>> &instructions_);
    void alloc_toA(shared_ptr<Instruction> instr, _toA *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocMove(shared_ptr<Instruction> instr, mov *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocCall(shared_ptr<Instruction> instr, call *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocDiv(shared_ptr<Instruction> instr, div *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocMod(shared_ptr<Instruction> instr, mod *i, vector<shared_ptr<Instruction>> &instructions_);
 
 };
  
  class RegisterAllocatorError : public std::runtime_error
  {
    public:
      RegisterAllocatorError(const char* err) : std::runtime_error(err) { }
  };
}
