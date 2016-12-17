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
    virtual std::string mnemonic() {
      return "X";
    }

    virtual std::string generate() {
      assert(false);
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
  
  class I2to1 : public Instruction {
  public:
    regNum dest;
    regNum src1;
    regNum src2;
    
    std::string generate() override {
      return mnemonic() + " " + std::to_string(src1) + ", " + std::to_string(src2);
    }
  };
  
  class I2to0 : public Instruction {
  public:
    regNum src1;
    regNum src2;
    
    std::string generate() override {
      return mnemonic() + " " + std::to_string(src1) + ", " + std::to_string(src2);
    }
  };
  
  class I1to0 : public Instruction {
  public:
    regNum src1;
  };
  
  class cmpl_ : public I2to0 {
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
          return "je";
          break;
          
        case ir_relation_greater:
          return "ja";
          break;
          
        case ir_relation_greater_equal:
          return "jae";
          break;
          
        case ir_relation_less:
          return "jb";
          break;
          
        case ir_relation_less_equal:
          return "jbe";
          break;
          
        case ir_relation_less_greater:
          return "jne";
          break;
          
        default:
          return "XX";
      }
    }
    
    std::string generate() override {
      return mnemonic() + " " + label;
    }
  };
  
  class jmp : public Instruction {
  public:
    Label label;
    std::string generate() override {
      return "jmp " + label;
    }
  };
  
  
  class movl_from_stack : public Instruction {
  public:
    unsigned offset = 0;
    regNum dest;
    
    std::string generate() override {
      return "movl " + std::to_string((signed)offset * -4) + "(%ebp), " + getRegisterName(dest);
    }
  };
  
  class movl_to_stack : public Instruction {
  public:
    unsigned offset = 0;
    regNum src;
    
    std::string generate() override {
      return "movl " + getRegisterName(src) + " "+ std::to_string((signed)offset * -4) + "(%ebp)";
    }
  };
  
  
  class movl_from_imm : public Instruction {
  public:
    long imm_value;
    regNum dest;
    
    std::string generate() override {
      return "movl $" + std::to_string(imm_value) + ", " + getRegisterName(dest);
    }
  };
  
  
  class movl_to_rax : public I1to0 {
  public:
    std::string generate() override {
      return "movl " + getRegisterName(src1) + ", %rax";
    }
  };
  
  
  class addl : public I2to1 {
    std::string mnemonic() override {
      return "addl";
    }
  };
  
  
  class imull : public I2to1 {
    std::string mnemonic() override {
      return "imull";
    }
  };
  
  class popq_rbp : public Instruction {
  public:
    std::string generate() override {
      return "popq %rbp";
    }
  };
  
  class retq : public Instruction {
  public:
    std::string generate() override {
      return "retq";
    }
  };
  
  class pushq_rbp : public Instruction {
  public:
    std::string generate() override {
      return "pushq %rbp";
    }
  };
  
  class movq_rsp_rbp : public Instruction {
  public:
    std::string generate() override {
      return "movq %rsp %rbp";
    }
  };
  
  class subq_rsp : public Instruction {
  public:
    unsigned nslots;
    std::string generate() override {
      return "subq " + std::to_string(nslots * 4) + " %rsp";
    }
  };



}
