#pragma once

#include "backend.h"

#include <libfirm/firm.h>
#include <string>
#include <assert.h>
#include <iostream>

namespace cmpl
{
  
  typedef long regNum;
  typedef std::string Label;
  
  // describes one instruction
  class Instruction
  {
  public:
    regNum dest;
    regNum src1;
    regNum src2;
    
    virtual std::string mnemonic() {
      return "NOP";
    }

    virtual std::string generate() {
//      assert(src2 == dest);
      return "\t" + mnemonic() + " " + getRegisterName(src1) + ", " + getRegisterName(src2);
    };

    
    static std::string getRegisterName(regNum reg)
    {
      return std::to_string(reg);
      
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
  
  class cmpl_ : public Instruction {
    std::string mnemonic() override {
      return "cmpl";
    }
  };
  
  class Branch : public Instruction {
  public:
    using Instruction::Instruction;
    Label label;
    ir_relation relation;
    
    std::string mnemonic() override {
      switch (relation) {
        case ir_relation_equal:
          return "JE";
          break;
          
        case ir_relation_greater:
          return "JA";
          break;
          
        case ir_relation_greater_equal:
          return "JAE";
          break;
          
        case ir_relation_less:
          return "JB";
          break;
          
        case ir_relation_less_equal:
          return "JBE";
          break;
          
        case ir_relation_less_greater:
          return "JNE";
          break;
          
        default:
          return "XX";
      }
    }
    
    std::string generate() override {
      return "\t" + mnemonic() + " " + label;
    }
  };
  
  class jmp : public Instruction {
  public:
    Label label;
    std::string generate() override {
      return "\tjmp " + label;
    }
  };
  
  class Lbl : public Instruction {
  public:
    using Instruction::Instruction;
    Label label;
    std::string generate() override {
      return label + ":";
    }
  };
  
  
  class movl_stack : public Instruction {
  public:
    unsigned offset = 0;
    using Instruction::Instruction;
    
    std::string generate() override {
      return "\tmovl " + std::to_string((signed)offset * -4) + "(%ebp), " + getRegisterName(dest);
    }
  };
  
  
  class movl_imm : public Instruction {
  public:
    using Instruction::Instruction;
    
    long imm_value;
    
    std::string generate() override {
      return "\tmovl $" + std::to_string(imm_value) + ", " + getRegisterName(dest);
    }
  };
  
  
  class movl_rax : public Instruction {
  public:
    using Instruction::Instruction;
    
    std::string generate() override {
      return "\tmovl " + getRegisterName(src1) + ", %rax";
    }
  };
  
  
  class addl : public Instruction {
    using Instruction::Instruction;
    std::string mnemonic() override {
      return "addl";
    }
  };
  
  
  class imull : public Instruction {
    using Instruction::Instruction;
    std::string mnemonic() override {
      return "imull";
    }
  };
  
  class popqrbp : public Instruction {
  public:
    using Instruction::Instruction;
    
    std::string generate() override {
      return "\tpopq %rbp";
    }
  };
  
  class retq : public Instruction {
  public:
    using Instruction::Instruction;
    
    std::string generate() override {
      return "\tretq";
    }
  };



}
