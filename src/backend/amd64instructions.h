#pragma once

#include <libfirm/firm.h>
#include <string>
#include <assert.h>
#include <iostream>

#ifdef _WIN32
  // TODO
#elif __APPLE__
  #define AMD64AssDirAllign ".p2align 4,0x90,15"
  #define AMD64LdNamePrefix "_"
  #define AMD64LabelPrefix "L"
#elif __linux__
  #define AMD64AssDirAllign ".p2align 4,,15"
  #define AMD64LdNamePrefix 
  #define AMD64LabelPrefix ".L"
#else
  #error "platform not supported"
#endif

namespace cmpl
{
  
  typedef long regNum;
  typedef std::string Label;
  
  static const char* const x86_64RegisterNames[] = 
  {
    "%eax", 
    "%ebx", 
    "%ecx", 
    "%edx", 
    // registers 4-7 have special purposes
    "%ebp", 
    "%esp", 
    "%esi", 
    "%edi", 
  };
  
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
      assert(reg < 4);
      // registers 4-7 must not be used as general purpose registers
      
      return std::string(x86_64RegisterNames[reg]);
    }
  };
  
  class I2to1 : public Instruction {
  public:
    regNum dest;
    regNum src1;
    regNum src2;
    
    std::string generate() override {
      return mnemonic() + " " + getRegisterName(src1) + ", " + getRegisterName(src2);
    }
  };
  
  class I2to0 : public Instruction {
  public:
    regNum src1;
    regNum src2;
    
    std::string generate() override {
      return mnemonic() + " " + getRegisterName(src1) + ", " + getRegisterName(src2);
    }
  };
  
  class I1to0 : public Instruction {
  public:
    regNum src1;
    
    std::string generate() override {
      return mnemonic() + " " + getRegisterName(src1);
    }
  };
  
  class I1to1 : public Instruction {
  public:
    regNum src1;
    regNum dest;
    
    std::string generate() override {
      return mnemonic() + " " + getRegisterName(src1) + ", " + std::to_string(dest);
    }
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
  
  
  class movl : public I1to1 {
    std::string mnemonic() override {
      return "movl";
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
      return "movl " + getRegisterName(src) + ", "+ std::to_string((signed)offset * -4) + "(%ebp)";
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
      return "movq %rsp, %rbp";
    }
  };
  
  class subq_rsp : public Instruction {
  public:
    unsigned nslots;
    std::string generate() override {
      return "subq " + std::to_string(nslots * 4) + ", %rsp";
    }
  };



}
