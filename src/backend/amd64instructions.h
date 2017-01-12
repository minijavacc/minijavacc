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

using namespace std;

namespace cmpl
{
  static int lastUsedRegisterIdentifier = 1000;
  static unsigned nallocated = 0;
  
  enum RegisterSize {
    RegisterSize64,
    RegisterSize32
  };
  
  enum RegisterType {
    RegisterTypeVirtual,
    RegisterTypePhysical
  };
  
  #define ID_AX 0
  #define ID_BX 1
  #define ID_CX 2
  #define ID_DX 3
  #define ID_BP 4
  #define ID_SP 5
  #define ID_SI 6
  #define ID_DI 7
  
  class Register {
  public:
    int identifier;
    RegisterSize size;
    RegisterType type;
    
    Register() {};
    
    Register(RegisterSize s) : size(s), type(RegisterTypeVirtual) {
      identifier = lastUsedRegisterIdentifier++;
      nallocated++;
    };
    
    Register(ir_mode *mode) : Register(registerSizeFromIRMode(mode)) {};
    
    string getRegisterName() {
      assert(type == RegisterTypePhysical);
      
      if (size == RegisterSize32) {
        switch (identifier) {
          case 0:
            return "%eax";
          case 1:
            return "%ebx";
          case 2:
            return "%ecx";
          case 3:
            return "%edx";
          case 4:
            return "%ebp";
          case 5:
            return "%rsp";
          case 6:
            return "%esi";
          case 7:
            return "%edi";
          default:
            assert(false);
            return 0;
        }
      } else {
        switch (identifier) {
          case 0:
            return "%rax";
          case 1:
            return "%rbx";
          case 2:
            return "%rcx";
          case 3:
            return "%rdx";
          case 4:
            return "%rbp";
          case 5:
            return "%rsp";
          case 6:
            return "%rsi";
          case 7:
            return "%rdi";
          default:
            assert(false);
            return 0;
        }
      }
    };
    
    static RegisterSize registerSizeFromIRMode(ir_mode *mode) {
      if (mode_is_int(mode)) return RegisterSize32;
      return RegisterSize64;
    }
    
    static unsigned numberOfDynamicRegisters() {
      return nallocated;
    }
    
    // TODO: make singleton
    static shared_ptr<Register> _ax(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_AX;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> _bx(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_BX;
      return r;
    };
    
    // ...
    
    // TODO: make singleton
    static shared_ptr<Register> eax() {
      auto r = make_shared<Register>();
      r->size = RegisterSize32;
      r->type = RegisterTypePhysical;
      r->identifier = ID_AX;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> ebx() {
      auto r = make_shared<Register>();
      r->size = RegisterSize32;
      r->type = RegisterTypePhysical;
      r->identifier = ID_BX;
      return r;
    };
    
    // ...
    
    // TODO: make singleton
    static shared_ptr<Register> rbp() {
      auto r = make_shared<Register>();
      r->size = RegisterSize64;
      r->type = RegisterTypePhysical;
      r->identifier = ID_BP;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> rsp() {
      auto r = make_shared<Register>();
      r->size = RegisterSize64;
      r->type = RegisterTypePhysical;
      r->identifier = ID_SP;
      return r;
    };
    
    // ...
    
  };
  
  typedef std::string Label;
  
  // describes one instruction
  class Instruction
  {
  protected:
    string size64suffix = "q";
    string size32suffix = "l";
    
  public:
    virtual std::string mnemonic() {
      return "X";
    }

    virtual std::string generate() {
      assert(false);
      return "";
    };
  };
  
  class I2to1 : public Instruction {
  public:
    shared_ptr<Register> dest;
    shared_ptr<Register> src1;
    shared_ptr<Register> src2;
    
    std::string generate() override {
      assert(src2->identifier == dest->identifier);
      
      string suffix;
      if (dest->size == RegisterSize64
          && src1->size == RegisterSize64
          && src2->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (dest->size == RegisterSize32
                 && src1->size == RegisterSize32
                 && src2->size == RegisterSize32) {
        suffix = size32suffix;
      } else {
        // TODO: throw IllegalRegisterConfigurationException
        assert(false);
      }
      return mnemonic() + suffix + " " + src1->getRegisterName() + ", " + src2->getRegisterName();
    }
  };
  
  class I2to0 : public Instruction {
  public:
    shared_ptr<Register> src1;
    shared_ptr<Register> src2;
    
    std::string generate() override {
      string suffix;
      if (src1->size == RegisterSize64
          && src2->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (src1->size == RegisterSize32
                 && src2->size == RegisterSize32) {
        suffix = size32suffix;
      } else {
        // TODO: throw IllegalRegisterConfigurationException
        assert(false);
      }
      return mnemonic() + suffix + " " + src1->getRegisterName() + ", " + src2->getRegisterName();
    }
  };
  
  class I1to0 : public Instruction {
  public:
    shared_ptr<Register> src1;
    
    std::string generate() override {
      string suffix;
      if (src1->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (src1->size == RegisterSize32) {
        suffix = size32suffix;
      }
      return mnemonic() + suffix + " " + src1->getRegisterName();
    }
  };
  
  class I0to1 : public Instruction {
  public:
    shared_ptr<Register> dest;
    
    std::string generate() override {
      string suffix;
      if (dest->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (dest->size == RegisterSize32) {
        suffix = size32suffix;
      }
      return mnemonic() + suffix + " " + dest->getRegisterName();
    }
  };
  
  class I1to1 : public Instruction {
  public:
    shared_ptr<Register> src1;
    shared_ptr<Register> dest;
    
    std::string generate() override {
      string suffix;
      if (src1->size == RegisterSize64
          && dest->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (src1->size == RegisterSize32
                 && dest->size == RegisterSize32) {
        suffix = size32suffix;
      } else {
        assert(false);
      }
      return mnemonic() + suffix + " " + src1->getRegisterName() + ", " + dest->getRegisterName();
    }
  };
  
  class cmp : public I2to0 {
    std::string mnemonic() override {
      return "cmp";
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
  
  
  class mov : public I1to1 {
    std::string mnemonic() override {
      return "mov";
    }
  };
  
  
  class add : public I2to1 {
    std::string mnemonic() override {
      return "add";
    }
  };
  
  class neg : public I1to1 {
    std::string mnemonic() override {
      return "neg";
    }
  };
  
  class sub : public I2to1 {
    std::string mnemonic() override {
      return "sub";
    }
  };
  
  class idiv : public I2to1 {
    // quotient in %eax, remainder in %edx
    std::string mnemonic() override {
      return "idiv";
    }
  };
  
  class imul : public I2to1 {
    std::string mnemonic() override {
      return "imul";
    }
  };
  
  class pop : public I0to1 {
  public:
    std::string mnemonic() override {
      return "pop";
    }
  };

  
  class push : public I1to0 {
  public:
    std::string mnemonic() override {
      return "push";
    }
  };
  
  
  // Special case, may be restructured later
  class retq : public Instruction {
  public:
    std::string generate() override {
      return "retq";
    }
  };
  
  
  // Special case, may be restructured later
  class subq_rsp : public Instruction {
  public:
    unsigned nslots;
    std::string generate() override {
      return "subq " + std::to_string(nslots * 4) + ", %rsp";
    }
  };
  
  
  // Special case, may be restructured later
  class mov_from_stack : public Instruction {
  public:
    long offset = 0; // In bytes
    shared_ptr<Register> dest;
    
    std::string generate() override {
      string suffix;
      if (dest->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (dest->size == RegisterSize32) {
        suffix = size32suffix;
      }
      return "mov" + suffix + " " + std::to_string(offset) + "(%ebp), " + dest->getRegisterName();
    }
  };
  
  
  // Special case, may be restructured later
  class mov_to_stack : public Instruction {
  public:
    long offset = 0; // In bytes
    shared_ptr<Register> src;
    
    std::string generate() override {
      string suffix;
      if (src->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (src->size == RegisterSize32) {
        suffix = size32suffix;
      }
      return "mov" + suffix + " " + src->getRegisterName() + ", "+ std::to_string((signed)offset) + "(%ebp)";
    }
  };
  
  
  // Special case, may be restructured later
  class mov_from_imm : public Instruction {
  public:
    long imm_value;
    shared_ptr<Register> dest;
    
    std::string generate() override {
      string suffix;
      if (dest->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (dest->size == RegisterSize32) {
        suffix = size32suffix;
      }
      return "mov" + suffix + " $" + std::to_string(imm_value) + ", " + dest->getRegisterName();
    }
  };
  
  
  class call : public Instruction {
  public:
    Label label;
    std::string generate() override {
      return "call " + label;
    }
  };



}
