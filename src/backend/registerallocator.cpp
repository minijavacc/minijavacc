#include "registerallocator.h"
#include "amd64instructions.h"
#include "values.h"
#include "assemblercontext.h"
#include <stdio.h>
#include <vector>
#include <libfirm/firm.h>


using namespace cmpl;

#pragma mark - Physical register allocation

//void RegisterAllocator::deliverValue(shared_ptr<Value> &src, shared_ptr<Value> &dest, vector<shared_ptr<Instruction>> &instructions)
//{
//  if (auto const& src_p = dynamic_cast<Virtual*>(src.get()))
//  {
//    if (auto const& dest_p = dynamic_cast<Physical*>(dest.get())) {
//      src.reset(dest_p);
//    }
//    
//    if (auto const& dest_p = dynamic_cast<Memory*>(dest.get())) {
//      src.reset(dest_p);
//    }
//    
//    if (auto const& dest_p = dynamic_cast<Virtual*>(dest.get())) {
//      allocValue(dest);
//      src.reset(dest_p);
//    }
//  }
//  
//  if (auto const& src_p = dynamic_cast<Immediate*>(src.get()))
//  {
//    if (auto const& dest_p = dynamic_cast<Physical*>(dest.get())) {
//      auto m = make_shared<mov>(__func__, __LINE__);
//      m->src1 = src;
//      m->dest = dest;
//      instructions.push_back(m);
//    }
//    
//    if (auto const& dest_p = dynamic_cast<Memory*>(dest.get())) {
//      auto m = make_shared<mov>(__func__, __LINE__);
//      m->src1 = src;
//      m->dest = dest;
//      instructions.push_back(m);
//    }
//    
//    if (auto const& dest_p = dynamic_cast<Virtual*>(dest.get())) {
//      dest.reset(src_p);
//    }
//  }
//  
//  if (auto const& src_p = dynamic_cast<Physical*>(src.get()))
//  {
//    if (auto const& dest_p = dynamic_cast<Physical*>(dest.get())) {
//      // check if same register => avoid mov
//      auto m = make_shared<mov>(__func__, __LINE__);
//      m->src1 = src;
//      m->dest = dest;
//      instructions.push_back(m);
//    }
//    
//    if (auto const& dest_p = dynamic_cast<Memory*>(dest.get())) {
//      auto m = make_shared<mov>(__func__, __LINE__);
//      m->src1 = src;
//      m->dest = dest;
//      instructions.push_back(m);
//    }
//    
//    if (auto const& dest_p = dynamic_cast<Virtual*>(dest.get())) {
//      dest.reset(src_p);
//    }
//  }
//  
//  if (auto const& src_p = dynamic_cast<Memory*>(src.get()))
//  {
//    if (auto const& dest_p = dynamic_cast<Physical*>(dest.get())) {
//      auto m = make_shared<mov>(__func__, __LINE__);
//      m->src1 = src;
//      m->dest = dest;
//      instructions.push_back(m);
//    }
//    
//    if (auto const& dest_p = dynamic_cast<Memory*>(dest.get())) {
//      // check if same => avoid mov
//      auto m1 = make_shared<mov>(__func__, __LINE__);
//      auto r10 = make_shared<Physical>(ID_10, m1->src1->getSize());
//      m1->src1 = src;
//      m1->dest = r10;
//      instructions.push_back(m1);
//      auto m2 = make_shared<mov>(__func__, __LINE__);
//      m2->src1 = r10;
//      m2->dest = dest;
//      instructions.push_back(m2);
//    }
//    
//    if (auto const& dest_p = dynamic_cast<Virtual*>(dest.get())) {
//      dest.reset(src_p);
//    }
//  }
//}


void RegisterAllocator::allocABtoB(shared_ptr<Instruction> instr, ABtoB *i, vector<shared_ptr<Instruction>> &instructions_)
{
  // the first operand (src1 and dest) must not be a memory location
  // because we never map two different values on the same stack frame
  
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  auto src2 = i->src2->getLowered(stackFrameAllocation);
  auto dest = i->dest->getLowered(stackFrameAllocation);
  
  shared_ptr<Physical> r = make_shared<Physical>(ID_10, src2->getSize());
  
  auto from_src_to_r = src2->movToPhysical(r);
  auto from_r_to_dest = dest->movFromPhysical(r);
  instructions_.push_back(from_src_to_r);
  instructions_.push_back(instr);
  instructions_.push_back(from_r_to_dest);
  
  i->src1 = src1;
  i->src2 = r;
  i->dest = r;
}

void RegisterAllocator::allocABto_(shared_ptr<Instruction> instr, ABto_ *i, vector<shared_ptr<Instruction>> &instructions_)
{
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  auto src2 = i->src2->getLowered(stackFrameAllocation);
  
  shared_ptr<Physical> r1 = make_shared<Physical>(ID_10, src1->getSize());
  
  // only one operand can be a memory access
  if (dynamic_cast<Memory*>(src1.get()) && dynamic_cast<Memory*>(src2.get())) {
    auto from_src_to_r = src1->movToPhysical(r1);
    instructions_.push_back(from_src_to_r);
    i->src1 = r1;
    i->src2 = src2;
  }
  
  // src2 must not be an immediate
  if (dynamic_cast<Immediate*>(src2.get())) {
    auto from_src_to_r = src2->movToPhysical(r1);
    instructions_.push_back(from_src_to_r);
    i->src1 = src1;
    i->src2 = r1;
  }
  
  // src2 as memory and src1 as immediate is not allowed
  if (dynamic_cast<Immediate*>(src1.get()) && dynamic_cast<Memory*>(src2.get())) {
    auto from_src_to_r = src2->movToPhysical(r1);
    instructions_.push_back(from_src_to_r);
    i->src1 = src1;
    i->src2 = r1;
  }
  
  instructions_.push_back(instr);
}

void RegisterAllocator::allocAtoA(shared_ptr<Instruction> instr, AtoA *i, vector<shared_ptr<Instruction>> &instructions_)
{
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  auto dest = i->dest->getLowered(stackFrameAllocation);
  
  shared_ptr<Physical> r1 = make_shared<Physical>(ID_10, src1->getSize());
  
  auto from_src_to_r = src1->movToPhysical(r1);
  instructions_.push_back(from_src_to_r);
  
  i->src1 = r1;
  i->dest = r1;
  
  instructions_.push_back(instr);
  
  auto from_r_to_d = dest->movFromPhysical(r1);
  instructions_.push_back(from_r_to_d);
}

void RegisterAllocator::allocAtoB(shared_ptr<Instruction> instr, AtoB *i, vector<shared_ptr<Instruction>> &instructions_)
{
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  auto dest = i->dest->getLowered(stackFrameAllocation);
  
  i->src1 = src1;
  i->dest = dest;
  
  // ony one operand can be a memory access
  if (dynamic_cast<Memory*>(src1.get()) && dynamic_cast<Memory*>(dest.get())) {
    shared_ptr<Physical> r1 = make_shared<Physical>(ID_10, src1->getSize());
    auto from_src_to_r = src1->movToPhysical(r1);
    instructions_.push_back(from_src_to_r);
    i->src1 = r1;
  }
  
  instructions_.push_back(instr);
}

void RegisterAllocator::allocAto_(shared_ptr<Instruction> instr, Ato_ *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  i->src1 = src1;
  
  instructions_.push_back(instr);
}

void RegisterAllocator::alloc_toA(shared_ptr<Instruction> instr, _toA *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  auto dest = i->dest->getLowered(stackFrameAllocation);
  i->dest = dest;
  
  instructions_.push_back(instr);
}

void RegisterAllocator::allocDiv(shared_ptr<Instruction> instr, div *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  shared_ptr<Physical> c = make_shared<Physical>(ID_AX, ValueSize32);
  shared_ptr<Physical> low = make_shared<Physical>(ID_AX, ValueSize32);
  
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  auto src2 = i->src2->getLowered(stackFrameAllocation);
  auto dest = i->dest->getLowered(stackFrameAllocation);
  
  i->src1 = src1;
  i->src2 = src2;
  i->dest = dest;
  
  auto from_src1_to_low = src1->movToPhysical(low);
  instructions_.push_back(from_src1_to_low);
  
  // Argument to idiv has to be register or memory location
  if (dynamic_cast<Immediate*>(src2.get())) {
    shared_ptr<Physical> r1 = make_shared<Physical>(ID_10, src2->getSize());
    auto from_src2_to_r = src2->movToPhysical(r1);
    instructions_.push_back(from_src2_to_r);
    i->src2 = r1;
  }
  
  auto cl = make_shared<cltd>(__func__, __LINE__);
  instructions_.push_back(cl);
  
  instructions_.push_back(instr);
  
  auto from_c_to_dest = dest->movFromPhysical(c);
  instructions_.push_back(from_c_to_dest);
}

void RegisterAllocator::allocMod(shared_ptr<Instruction> instr, mod *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  shared_ptr<Physical> c = make_shared<Physical>(ID_DX, ValueSize32);
  shared_ptr<Physical> low = make_shared<Physical>(ID_AX, ValueSize32);
  
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  auto src2 = i->src2->getLowered(stackFrameAllocation);
  auto dest = i->dest->getLowered(stackFrameAllocation);
  
  i->src1 = src1;
  i->src2 = src2;
  i->dest = dest;
  
  auto from_src1_to_low = src1->movToPhysical(low);
  instructions_.push_back(from_src1_to_low);
  
  // Argument to idiv has to be register or memory location
  if (dynamic_cast<Immediate*>(src2.get())) {
    shared_ptr<Physical> r1 = make_shared<Physical>(ID_10, src2->getSize());
    auto from_src2_to_r = src2->movToPhysical(r1);
    instructions_.push_back(from_src2_to_r);
    i->src2 = r1;
  }

  auto cl = make_shared<cltd>(__func__, __LINE__);
  instructions_.push_back(cl);

  instructions_.push_back(instr);
  
  auto from_c_to_dest = dest->movFromPhysical(c);
  instructions_.push_back(from_c_to_dest);
}

void RegisterAllocator::allocCall(shared_ptr<Instruction> instr, call *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  // if function returns void, i->dest is NULL
  
  instructions_.push_back(instr);
  
  if (i->dest != nullptr)
  {
    // move result to stack
    auto dest = i->dest->getLowered(stackFrameAllocation);
    i->dest = dest;
    shared_ptr<Physical> ax = make_shared<Physical>(ID_AX, dest->getSize());
    auto from_ax_to_dest = dest->movFromPhysical(ax);
    instructions_.push_back(from_ax_to_dest);
  }
}

void RegisterAllocator::allocMove(shared_ptr<Instruction> instr, mov *i, vector<shared_ptr<Instruction>> &instructions_)
{
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  auto dest = i->dest->getLowered(stackFrameAllocation);
  
  i->src1 = src1;
  i->dest = dest;
  
  if (dynamic_cast<Memory*>(src1.get()) && dynamic_cast<Memory*>(dest.get())) {
    shared_ptr<Physical> r1 = make_shared<Physical>(ID_10, src1->getSize());
    auto from_src_to_r = src1->movToPhysical(r1);
    instructions_.push_back(from_src_to_r);
    i->src1 = r1;
  }
  
  instructions_.push_back(instr);
}

void RegisterAllocator::allocMovsx_rax(shared_ptr<Instruction> instr, movsxd_rax *i, vector<shared_ptr<Instruction>> &instructions_)
{
  auto src1 = i->src1->getLowered(stackFrameAllocation);
  i->src1 = src1;
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
      if (auto i = dynamic_cast<ABtoB*>(instruction.get())) {
        allocABtoB(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<div*>(instruction.get())) {
        allocDiv(instruction, i, instructions_);		
      } else if (auto i = dynamic_cast<mod*>(instruction.get())) {
        allocMod(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<ABto_*>(instruction.get())) {
        allocABto_(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<AtoA*>(instruction.get())) {
        allocAtoA(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<AtoB*>(instruction.get())) {
        allocAtoB(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<Ato_*>(instruction.get())) {
        allocAto_(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<_toA*>(instruction.get())) {
        alloc_toA(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<call*>(instruction.get())) {
        allocCall(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<mov*>(instruction.get())) {
        allocMove(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<movsxd_rax*>(instruction.get())) {
        allocMovsx_rax(instruction, i, instructions_);
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
