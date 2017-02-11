//
//  values.cpp
//  mjcc
//
//  Created by Markus Schlegel on 04/02/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#include "values.h"
#include "amd64instructions.h"
#include <memory>
#include "assemblercontext.h"
#include <map>

using namespace cmpl;
using namespace std;
  

// Base Value class
// ------------------

string Value::toString() {
  assert(false);
  return "";
};

ValueSize Value::getSize() {
  return size;
}

void Value::setSize(ValueSize s) {
  size = s;
}

ValueSize Value::valueSizeFromIRNode(ir_node *node) {
  ir_mode *mode;
  
  if (is_Load(node)) {
    mode = get_Load_mode(node);
    
  } else if (is_Call(node)) {
    auto t = get_Call_type(node);
    auto ft = get_method_res_type(t, 0);
    mode = get_type_mode(ft);
    
  } else if (is_Mod(node)) {
    mode = get_Mod_resmode(node);
    
  } else if (is_Div(node)) {
    mode = get_Div_resmode(node);
    
  } else {
    mode = get_irn_mode(node);
  }
  
  return valueSizeFromIRMode(mode);
}

ValueSize Value::valueSizeFromIRMode(ir_mode *mode) {
  if (mode_is_reference(mode)) return ValueSize64;
  if (mode_is_int(mode))
  {
    if (get_mode_size_bits(mode) == 32)
      return ValueSize32;
    if (get_mode_size_bits(mode) == 64)
      return ValueSize64;
  }
  
  assert(false);
  return ValueSizeUndefined;
}

shared_ptr<Value> Value::getLowered(shared_ptr<StackFrameAllocation> allocation) {
  return shared_from_this();
}



// Immediate Value class
// ------------------

Immediate::Immediate(long i, ir_mode *mode) : immediate(i) {
  this->size = valueSizeFromIRMode(mode);
};

string Immediate::toString() {
  return "$" + to_string(immediate); // decimal
}

shared_ptr<cmpl::mov> Immediate::movToPhysical(shared_ptr<Physical> p) {
  auto m = make_shared<mov>(__func__, __LINE__);
  m->src1 = static_pointer_cast<Immediate>(shared_from_this());
  m->dest = p;
  
  return m;
}



// Physical register Value class
// ------------------

Physical::Physical(long identifier, ValueSize s) : identifier(identifier) {
  this->size = s;
}

string Physical::toString() {
  if (size == ValueSize32) {
    switch (identifier) {
      case ID_AX:
        return "%eax";
      case ID_BX:
        return "%ebx";
      case ID_CX:
        return "%ecx";
      case ID_DX:
        return "%edx";
      case ID_BP:
        return "%ebp";
      case ID_SP:
        return "%rsp"; // esp?
      case ID_SI:
        return "%esi";
      case ID_DI:
        return "%edi";
      case ID_08:
        return "%r8d";
      case ID_09:
        return "%r9d";
      case ID_10:
        return "%r10d";
      case ID_11:
        return "%r11d";
      case ID_12:
        return "%r12d";
      case ID_13:
        return "%r13d";
      case ID_14:
        return "%r14d";
      case ID_15:
        return "%r15d";
      default:
        assert(false);
        return 0;
    }
  } else {
    switch (identifier) {
      case ID_AX:
        return "%rax";
      case ID_BX:
        return "%rbx";
      case ID_CX:
        return "%rcx";
      case ID_DX:
        return "%rdx";
      case ID_BP:
        return "%rbp";
      case ID_SP:
        return "%rsp";
      case ID_SI:
        return "%rsi";
      case ID_DI:
        return "%rdi";
      case ID_08:
        return "%r8";
      case ID_09:
        return "%r9";
      case ID_10:
        return "%r10";
      case ID_11:
        return "%r11";
      case ID_12:
        return "%r12";
      case ID_13:
        return "%r13";
      case ID_14:
        return "%r14";
      case ID_15:
        return "%r15";
      default:
        assert(false);
        return 0;
    }
  }
}

shared_ptr<cmpl::mov> Physical::movToPhysical(shared_ptr<Physical> p) {
  auto m = make_shared<cmpl::mov>(__func__, __LINE__);
  m->src1 = static_pointer_cast<Physical>(shared_from_this());
  m->dest = p;
  
  return m;
}

shared_ptr<cmpl::mov> Physical::movFromPhysical(shared_ptr<Physical> p) {
  auto m = make_shared<cmpl::mov>(__func__, __LINE__);
  m->src1 = p;
  m->dest = static_pointer_cast<Physical>(shared_from_this());
  
  return m;
}



// Memory location Value class
// ------------------

Memory::Memory(shared_ptr<Physical> pointer, int offset, ValueSize size) : pointer(pointer), offset(offset) {
  this->size = size;
}
  
string Memory::toString() {
  return to_string(offset) + "(" + pointer->toString() + ")";
}

shared_ptr<mov> Memory::movToPhysical(shared_ptr<Physical> p) {
  auto m = make_shared<mov>(__func__, __LINE__);
  m->src1 = static_pointer_cast<Memory>(shared_from_this());
  m->dest = p;
  
  return m;
}

shared_ptr<mov> Memory::movFromPhysical(shared_ptr<Physical> p) {
  auto m = make_shared<mov>(__func__, __LINE__);
  m->src1 = p;
  m->dest = static_pointer_cast<Memory>(shared_from_this());
  
  return m;
}





// Virtual register Value class
// ------------------

Virtual::Virtual(ValueSize s) {
  this->size = s;
  identifier = lastUsedRegisterIdentifier++;
}

Virtual::Virtual(ir_node *node) : Virtual(valueSizeFromIRNode(node)) {};

string Virtual::toString() {
  return "v" + std::to_string(identifier);
}

shared_ptr<Value> Virtual::getLowered(shared_ptr<StackFrameAllocation> allocation) {
  auto rbp = make_shared<Physical>(ID_BP, ValueSize64);
  int offset;
  
  if (allocation->allocations.count(shared_from_this())) {
    offset = allocation->allocations[shared_from_this()];
  } else {
    offset = allocation->allocate(shared_from_this());
  }
  
  auto m = make_shared<Memory>(rbp, offset, size);
  return m;
}


