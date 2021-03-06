//
//  amd64instructions.cpp
//  mjcc
//
//  Created by Markus Schlegel on 04/02/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#include "amd64instructions.h"
#include "values.h"

using namespace cmpl;
using namespace std;



// Instruction
// -----------

string Instruction::mnemonic() {
  return "X";
}

string Instruction::annotation() {
  if (node_nr < 0)
    return string(fnc) + ":" + to_string(line);
  else
    return string(fnc) + ":" + to_string(line) + " (#" + to_string(node_nr) + ")";
}

string Instruction::generate() {
  assert(false);
  return "";
};



// ABtoA
// -----------

string ABtoB::generate() {
  // TODO: removed the assert to allow output of virtual registers
  //assert(src2->identifier == dest->identifier);
  
  string suffix;
  if (dest->getSize() == ValueSize64
      && src1->getSize() == ValueSize64
      && src2->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (dest->getSize() == ValueSize32
             && src1->getSize() == ValueSize32
             && src2->getSize() == ValueSize32) {
    suffix = size32suffix;
  } else {
    // TODO: throw IllegalRegisterConfigurationException
    assert(false);
  }
  return mnemonic() + suffix + " " + src1->toString() + ", " + src2->toString() + "\t\t\t# " + annotation();
}


// ABto_
// -----------

string ABto_::generate() {
  string suffix;
  if (src1->getSize() == ValueSize64
      && src2->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (src1->getSize() == ValueSize32
             && src2->getSize() == ValueSize32) {
    suffix = size32suffix;
  } else {
    // TODO: throw IllegalRegisterConfigurationException
    assert(false);
  }
  return mnemonic() + suffix + " " + src1->toString() + ", " + src2->toString() + "\t\t\t# " + annotation();
}


// Ato_
// -----------

string Ato_::generate() {
  string suffix;
  if (src1->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (src1->getSize() == ValueSize32) {
    suffix = size32suffix;
  }
  return mnemonic() + suffix + " " + src1->toString() + "\t\t\t# " + annotation();
}


// _toA
// -----------

string _toA::generate() {
  string suffix;
  if (dest->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (dest->getSize() == ValueSize32) {
    suffix = size32suffix;
  }
  return mnemonic() + suffix + " " + dest->toString() + "\t\t\t# " + annotation();
}


// AtoA
// -----------

string AtoA::generate() {
  string suffix;
  if (src1->getSize() == ValueSize64
      && dest->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (src1->getSize() == ValueSize32
             && dest->getSize() == ValueSize32) {
    suffix = size32suffix;
  } else {
    assert(false);
  }
  return mnemonic() + suffix + " " + dest->toString() + "\t\t\t# " + annotation();
}


// AtoB
// -----------

string AtoB::generate() {
  string suffix;
  if (src1->getSize() == ValueSize64
      && dest->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (src1->getSize() == ValueSize32
             && dest->getSize() == ValueSize32) {
    suffix = size32suffix;
  } else {
    assert(false);
  }
  return mnemonic() + suffix + " " + dest->toString() + "\t\t\t# " + annotation();
}


// cmp
// -----------

string cmp::mnemonic() {
  return "cmp";
}


// Branch
// -----------

string Branch::mnemonic() {
  switch (relation) {
    case ir_relation_equal:
      return "je";
      break;
      
    case ir_relation_greater:
      return "jg";
      break;
      
    case ir_relation_greater_equal:
      return "jge";
      break;
      
    case ir_relation_less:
      return "jl";
      break;
      
    case ir_relation_less_equal:
      return "jle";
      break;
      
    case ir_relation_less_greater:
      return "jne";
      break;
      
    default:
      return "XX";
  }
}

string Branch::generate() {
  return mnemonic() + " " + label + "\t\t\t# " + annotation();;
}


// jmp
// -----------

string jmp::generate() {
  return "jmp " + label + "\t\t\t# " + annotation();;
}


// mov
// -----------


string mov::mnemonic() {
  return "mov";
}

string mov::generate() {
  string suffix;
  
  if (src1->getSize() == ValueSize64
      && dest->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (src1->getSize() == ValueSize32
             && dest->getSize() == ValueSize32) {
    suffix = size32suffix;
  } else {
    assert(false);
  }
  
  return mnemonic() + suffix + " " + src1->toString() + ", " + dest->toString() + "\t\t\t# " + annotation();
}


// movsxd_rax
// -----------

string movsxd_rax::generate() {
  assert(src1->getSize() == ValueSize32);
  return "movslq " + src1->toString() + ", %rax" + "\t\t\t# " + annotation();
}


// add
// -----------

string add::mnemonic() {
  return "add";
}


// neg
// -----------

string neg::mnemonic() {
  return "neg";
}


// sub
// -----------

string sub::mnemonic() {
  return "sub";
}


// div
// -----------

string div::mnemonic() {
  return "idiv";
}

string div::generate() {
  string suffix;
  
  if (dest->getSize() == ValueSize64
      && src1->getSize() == ValueSize64
      && src2->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (dest->getSize() == ValueSize32
             && src1->getSize() == ValueSize32
             && src2->getSize() == ValueSize32) {
    suffix = size32suffix;
  } else {
    assert(false);
  }
  
  return mnemonic() + suffix + " " + src2->toString() + "\t\t\t# " + annotation();
}


// mod
// -----------

string mod::mnemonic() {
  return "idiv";
}

string mod::generate() {
  string suffix;
  
  if (dest->getSize() == ValueSize64
      && src1->getSize() == ValueSize64
      && src2->getSize() == ValueSize64) {
    suffix = size64suffix;
  } else if (dest->getSize() == ValueSize32
             && src1->getSize() == ValueSize32
             && src2->getSize() == ValueSize32) {
    suffix = size32suffix;
  } else {
    assert(false);
  }
  
  return mnemonic() + suffix + " " + src2->toString() + "\t\t\t# " + annotation();
}


// imul
// -----------

string imul::mnemonic() {
  return "imul";
}


// pop
// -----------

string pop::mnemonic() {
  return "pop";
}


// push
// -----------

string push::mnemonic() {
  return "push";
}


// ret
// -----------

string ret::generate() {
  return string("ret") + "\t\t\t# " + annotation();
}


// subq_rsp
// -----------

string subq_rsp::generate() {
  return "subq $" + std::to_string(bytes) + ", %rsp" + "\t\t\t# " + annotation();
}


// addq_rsp
// -----------

string addq_rsp::generate() {
  return "addq $" + std::to_string(bytes) + ", %rsp" + "\t\t\t# " + annotation();
}


// cltd
// -----------

string cltd::generate() {
  return "cltd";
}


// call
// -----------

string call::generate() {
  return "call " + label + "\t\t\t# " + annotation();;
}


// StaticInstruction
// -----------

string StaticInstruction::generate() {
  return str + "\t\t\t# " + annotation();
};
