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
    RegisterAllocator(shared_ptr<map<Label, shared_ptr<LabeledBlock>>> blocks, shared_ptr<vector<Label>> labels) : blocks(blocks), labels(labels) {};
    
    void run();
    
    
    // Only interface to graphassembler
    shared_ptr<map<Label, shared_ptr<LabeledBlock>>> blocks;
    shared_ptr<vector<Label>> labels; // topological order
    long topOfStack;
   
    void allocValue(shared_ptr<Value> &r);
    void deliverValue(shared_ptr<Value> &src, shared_ptr<Value> &dest, vector<shared_ptr<Instruction>> &instructions_);
    void allocI2to1(shared_ptr<Instruction> instr, I2to1 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI2to0(shared_ptr<Instruction> instr, I2to0 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI1to1(shared_ptr<Instruction> instr, I1to1 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI1to0(shared_ptr<Instruction> instr, I1to0 *i, vector<shared_ptr<Instruction>> &instructions_);
    void allocI0to1(shared_ptr<Instruction> instr, I0to1 *i, vector<shared_ptr<Instruction>> &instructions_);
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
