#include "registerallocator.h"
#include <stdio.h>
#include <vector>
#include <libfirm/firm.h>


using namespace cmpl;

#pragma mark - Misc

void RegisterAllocator::allocValue(shared_ptr<Value> &r) {
  if (r->type == ValueTypeVirtual)
  {
    // decrement global topOfStack to get new offset
    topOfStack -= 8;
    
    r->type = ValueTypeStackSlot;
    r->offset = topOfStack;
  }
}

#pragma mark - Physical register allocation

void RegisterAllocator::deliverValue(shared_ptr<Value> &src, shared_ptr<Value> &dest, vector<shared_ptr<Instruction>> &instructions)
{
  switch (src->type)
  {
    case ValueTypeVirtual:
      switch (dest->type)
      {
        case ValueTypePhysical:
        {
          src->type = dest->type;
          src->identifier = dest->identifier;
          break;
        }
          
        case ValueTypeStackSlot:
        {
          src->type = dest->type;
          src->offset = dest->offset;
          break;
        }
          
        case ValueTypeVirtual:
        {
          allocValue(dest);
          src->type = dest->type;
          src->offset = dest->offset;
          break;
        }
        
        default:
          assert(false);
      }
      break;
    
    case ValueTypeImmediate:
      switch (dest->type)
      {
        case ValueTypePhysical:
        case ValueTypeStackSlot:
        {
          auto m = make_shared<mov>(__func__, __LINE__);
          m->src1 = src;
          m->dest = dest;
          instructions.push_back(m);
          break;
        }
          
        case ValueTypeVirtual:
        {
          dest->type = src->type;
          dest->immediate = src->immediate;
          break;
        }
        
        default:
          assert(false);
      }
      break;
    
    case ValueTypePhysical:
      switch (dest->type)
      {
        case ValueTypePhysical:
        {
          if (src->identifier == dest->identifier)
            return;
          // no break
        }
        case ValueTypeStackSlot:
        {
          auto m = make_shared<mov>(__func__, __LINE__);
          m->src1 = src;
          m->dest = dest;
          instructions.push_back(m);
          break;
        }
          
        case ValueTypeVirtual:
        {
          dest->type = src->type;
          dest->offset = src->offset;
          break;
        }
        
        default:
          assert(false);
      }
      break;
    
    case ValueTypeStackSlot:
      switch (dest->type)
      {
        case ValueTypePhysical:
        {
          auto m = make_shared<mov>(__func__, __LINE__);
          m->src1 = src;
          m->dest = dest;
          instructions.push_back(m);
          break;
        }
        
        case ValueTypeStackSlot:
        {
          if (src->offset == dest->offset)
            return;
          auto m1 = make_shared<mov>(__func__, __LINE__);
          m1->src1 = src;
          m1->dest = Value::r10_(m1->src1->size);
          instructions.push_back(m1);
          auto m2 = make_shared<mov>(__func__, __LINE__);
          m2->src1 = Value::r10_(m1->src1->size);
          m2->dest = dest;
          instructions.push_back(m2);
          break;
        }
          
        case ValueTypeVirtual:
        {
          dest->type = src->type;
          dest->offset = src->offset;
          break;
        }
        
        default:
          assert(false);
      }
      break;
    
    default:
      assert(false);
  }
}


void RegisterAllocator::allocI2to1(shared_ptr<Instruction> instr, I2to1 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  // the first operand (src1 and dest) must not be a memory location
  // because we never map two different values on the same stack frame
  
  auto r = Value::r10_(i->src1->size);
  
  deliverValue(i->src1, r, instructions_);
  instructions_.push_back(instr);
  deliverValue(r, i->dest, instructions_);
  
  allocValue(i->src2);
  
  i->src1 = r;
  i->dest = r;
}

void RegisterAllocator::allocI2to0(shared_ptr<Instruction> instr, I2to0 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  allocValue(i->src1);
  allocValue(i->src2);
  
  // ony one operand can be a memory access
  if (i->src1->type == ValueTypeStackSlot && 
      i->src2->type == ValueTypeStackSlot)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src1, r1, instructions_);
    i->src1 = r1;
  }
  
  // src2 must not be an immediate
  if (i->src2->type == ValueTypeImmediate)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src2, r1, instructions_);
    i->src2 = r1;
  }
  
  // src2 as memory and src1 as immediate is not allowed
  if (i->src2->type == ValueTypeStackSlot && 
      i->src1->type == ValueTypeImmediate)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src2, r1, instructions_);
    i->src2 = r1;
  }
  
  instructions_.push_back(instr);
}

void RegisterAllocator::allocI1to1(shared_ptr<Instruction> instr, I1to1 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  allocValue(i->src1);
  allocValue(i->dest);
  
  // ony one operand can be a memory access
  if (i->src1->type == ValueTypeStackSlot && 
      i->dest->type == ValueTypeStackSlot)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src1, r1, instructions_);
    i->src1 = r1;
  }
  
  instructions_.push_back(instr);
}

void RegisterAllocator::allocI1to0(shared_ptr<Instruction> instr, I1to0 *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  allocValue(i->src1);
  
  instructions_.push_back(instr);
}

void RegisterAllocator::allocI0to1(shared_ptr<Instruction> instr, I0to1 *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  allocValue(i->dest);
  
  instructions_.push_back(instr);
}

void RegisterAllocator::allocDiv(shared_ptr<Instruction> instr, div *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  auto quotient = Value::eax();
  auto low = Value::eax();
  auto divisor = i->src1;
  auto dividend = i->src2;
  
  allocValue(i->src1);
  allocValue(i->src2);
  allocValue(i->result);
  
  deliverValue(dividend, low, instructions_);
  //reset high (edx) 
  auto cl = make_shared<cltd>(__func__, __LINE__);
  instructions_.push_back(cl);   
  instructions_.push_back(instr);
  deliverValue(quotient, i->result, instructions_);
}

void RegisterAllocator::allocMod(shared_ptr<Instruction> instr, mod *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  auto remainder = Value::edx();
  auto low = Value::eax();
  auto divisor = i->src1;
  auto dividend = i->src2;
  
  allocValue(i->src1);
  allocValue(i->src2);
  allocValue(i->result);
  
  deliverValue(dividend, low, instructions_);
  //reset high (edx) 
  auto cl = make_shared<cltd>(__func__, __LINE__);
  instructions_.push_back(cl);   
  instructions_.push_back(instr);
  deliverValue(remainder, i->result, instructions_);
}

void RegisterAllocator::allocCall(shared_ptr<Instruction> instr, call *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  // if function returns void, i->dest is NULL
  
  instructions_.push_back(instr);
  
  if (i->dest != nullptr)
  {
    // move result to stack
    auto ax = Value::_ax(i->dest->size);
    deliverValue(ax, i->dest, instructions_);
  }
}

void RegisterAllocator::allocMove(shared_ptr<Instruction> instr, mov *i, vector<shared_ptr<Instruction>> &instructions_)
{
  allocValue(i->src1);
  allocValue(i->dest);
  
  if (i->src1->type == ValueTypeStackSlot && 
      i->dest->type == ValueTypeStackSlot)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src1, r1, instructions_);
    i->src1 = r1;
  }
  
  instructions_.push_back(instr);
}


void RegisterAllocator::run()
{
  // work with instructions-vector
  // primitive: just use 2 registers
  for (auto const& label : *labels) {
    auto lb = blocks->at(label);
    auto instructions = lb->instructions;
    auto instructions_ = vector<shared_ptr<Instruction>>();
    
    for (auto const& instruction : instructions) {
      if (auto i = dynamic_cast<I2to1*>(instruction.get())) {
        allocI2to1(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<div*>(instruction.get())) {
        allocDiv(instruction, i, instructions_);		
      } else if (auto i = dynamic_cast<mod*>(instruction.get())) {
        allocMod(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<I2to0*>(instruction.get())) {
        allocI2to0(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<I1to1*>(instruction.get())) {
        allocI1to1(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<I1to0*>(instruction.get())) {
        allocI1to0(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<I0to1*>(instruction.get())) {
        allocI0to1(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<call*>(instruction.get())) {
        allocCall(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<mov*>(instruction.get())) {
        allocMove(instruction, i, instructions_);
      } else {
        instructions_.push_back(instruction);
      }
    }
    
    lb->instructions = std::move(instructions_);
  }
  
  // enhanced: from https://en.wikipedia.org/wiki/X86_calling_conventions#x86-64_calling_conventions
  // System V AMD64: 6 times int/pointer {RDI, RSI, RDX, RCX, R8, R9} ["certain" floating point {XMM0–7} probably irrelevant for us] Caller Stack aligned on 16 bytes
  // after the allocation only 6 registers must be used
  // save size of needed stack frame somewhere
}
