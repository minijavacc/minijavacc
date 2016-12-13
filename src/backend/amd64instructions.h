#pragma once

#include "backend.h"

#include <libfirm/firm.h>
#include <string>
#include <assert.h>

namespace cmpl
{
  
  typedef unsigned int regNum;
  typedef std::string Label;
  
  // describes one instruction
  class Instruction
  {
  public:
    ir_node* node;  // pointer to ir_node (can be ommitted later?)
    
    virtual std::string generate();
    
    std::string getRegisterName(regNum reg)
    {
      switch (reg)
      {
      case 0:
        return "%eax";
        break;
        
      case 1:
        return "%ebx";
        break;
      
      // TODO: include all available amd64 registers
      
      default:
        // TODO: solve forward declarations problem here
        //throw BackendError("can not map register to hardware");
        assert(false);
      }
    }
  };
  
  class AddInstr : Instruction
  {
  public:
    AddInstr(regNum src1, regNum src2, regNum dest)
      : src1(src1), src2(src2), dest(dest) {}
    
    std::string generate()
    {
      // TODO: just example with bla bla code!!!
      return "add " + getRegisterName(dest) + ", " + getRegisterName(src1) + ", " + getRegisterName(src2);
    }
    
    regNum dest;
    regNum src1;
    regNum src2;
  };
  
  class JmpInstr : Instruction
  {
  public:
    JmpInstr(Label label)
      : label(label) {}
    
    std::string generate()
    {
      // TODO: just example with bla bla code!!!
      return "jmp " + label;
    }
    
    Label label;
  };

}
