#pragma once

#include <libfirm/firm.h>
#include <string>
#include <assert.h>
#include <memory>
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
  #define ID_08 8
  #define ID_09 9
  #define ID_10 10
  #define ID_11 11
  #define ID_12 12
  #define ID_13 13
  #define ID_14 14
  #define ID_15 15
  
  class Register {
  public:
    int identifier;
    RegisterSize size;
    RegisterType type;
    
    Register() {};
    
    Register(RegisterSize s) : size(s), type(RegisterTypeVirtual) {
      identifier = lastUsedRegisterIdentifier++;
    };
    
    Register(ir_mode *mode) : Register(registerSizeFromIRMode(mode)) {};
    
    string getRegisterName() {
      if (type == RegisterTypePhysical) {
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
            case 8:
              return "%r8d";
            case 9:
              return "%r9d";
            case 10:
              return "%r10d";
            case 11:
              return "%r11d";
            case 12:
              return "%r12d";
            case 13:
              return "%r13d";
            case 14:
              return "%r14d";
            case 15:
              return "%r15d";
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
            case 8:
              return "%r8";
            case 9:
              return "%r9";
            case 10:
              return "%r10";
            case 11:
              return "%r11";
            case 12:
              return "%r12";
            case 13:
              return "%r13";
            case 14:
              return "%r14";
            case 15:
              return "%r15";
            default:
              assert(false);
              return 0;
          }
        }
      } else {
        return "v" + std::to_string(identifier);
      }
    };
    
    static RegisterSize registerSizeFromIRMode(ir_mode *mode) {
      if (mode_is_int(mode)) return RegisterSize32;
      return RegisterSize64;
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
    
    // TODO: make singleton
    static shared_ptr<Register> _cx(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_CX;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> _dx(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_DX;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> _di(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_DI;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> _si(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_SI;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> r8_(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_08;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> r9_(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_09;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> r10_(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_10;
      return r;
    };
    
    // TODO: make singleton
    static shared_ptr<Register> r11_(RegisterSize s) {
      auto r = make_shared<Register>();
      r->size = s;
      r->type = RegisterTypePhysical;
      r->identifier = ID_11;
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
    static shared_ptr<Register> edx() {
      auto r = make_shared<Register>();
      r->size = RegisterSize32;
      r->type = RegisterTypePhysical;
      r->identifier = ID_DX;
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
  private:
    int line;
    const char *fnc;
    
  protected:
    string size64suffix = "q";
    string size32suffix = "l";
    
  public:
    Instruction(const char *fnc, int line) {
      this->fnc = fnc;
      this->line = line;
    }
    
    virtual std::string mnemonic() {
      return "X";
    }
    
    virtual std::string annotation() {
      return string(fnc) + ":" + to_string(line);
    }

    virtual std::string generate() {
      assert(false);
      return "";
    };
  };
  
  class I2to1 : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Register> dest;
    shared_ptr<Register> src1;
    shared_ptr<Register> src2;
    
    std::string generate() override {
      // TODO: removed the assert to allow output of virtual registers
      //assert(src2->identifier == dest->identifier);
      
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
      return mnemonic() + suffix + " " + src1->getRegisterName() + ", " + src2->getRegisterName() + "\t\t\t# " + annotation();
    }
  };
    
  class I2to0 : public Instruction {
    using Instruction::Instruction;
    
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
      return mnemonic() + suffix + " " + src1->getRegisterName() + ", " + src2->getRegisterName() + "\t\t\t# " + annotation();
    }
  };
  
  class I1to0 : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Register> src1;
    
    std::string generate() override {
      string suffix;
      if (src1->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (src1->size == RegisterSize32) {
        suffix = size32suffix;
      }
      return mnemonic() + suffix + " " + src1->getRegisterName() + "\t\t\t# " + annotation();
    }
  };
  
  class I0to1 : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Register> dest;
    
    std::string generate() override {
      string suffix;
      if (dest->size == RegisterSize64) {
        suffix = size64suffix;
      } else if (dest->size == RegisterSize32) {
        suffix = size32suffix;
      }
      return mnemonic() + suffix + " " + dest->getRegisterName() + "\t\t\t# " + annotation();
    }
  };
  
  class I1to1 : public Instruction {
    using Instruction::Instruction;
    
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
      return mnemonic() + suffix + " " + src1->getRegisterName() + ", " + dest->getRegisterName() + "\t\t\t# " + annotation();
    }
  };
  
  class cmp : public I2to0 {
    using I2to0::I2to0;
    
    std::string mnemonic() override {
      return "cmp";
    }
  };
  
  class Branch : public Instruction {
    using Instruction::Instruction;
    
  public:
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
      return mnemonic() + " " + label + "\t\t\t# " + annotation();;
    }
  };
  
  class jmp : public Instruction {
    using Instruction::Instruction;
    
  public:
    Label label;
    std::string generate() override {
      return "jmp " + label + "\t\t\t# " + annotation();;
    }
  };
  
  
  class mov : public I1to1 {
    using I1to1::I1to1;
    
    std::string mnemonic() override {
      return "mov";
    }
  };
  
  
  class add : public I2to1 {
    using I2to1::I2to1;
    
    std::string mnemonic() override {
      return "add";
    }
  };
  
  class neg : public I1to1 {
    using I1to1::I1to1;
    
    std::string mnemonic() override {
      return "neg";
    }
  };
  
  class sub : public I2to1 {
    using I2to1::I2to1;
    
    std::string mnemonic() override {
      return "sub";
    }
  };
  
  class div : public I2to0 {
    using I2to0::I2to0;
    
    // result in %eax
    std::string mnemonic() override {
      return "idiv";
    }
  };
  
  class mod : public I2to0 {
    using I2to0::I2to0;
    
    // result in %edx
    std::string mnemonic() override {
      return "idiv";
    }
  };
  
  class imul : public I2to1 {
    using I2to1::I2to1;
    
    std::string mnemonic() override {
      return "imul";
    }
  };
  
  class pop : public I0to1 {
    using I0to1::I0to1;
    
  public:
    std::string mnemonic() override {
      return "pop";
    }
  };

  
  class push : public I1to0 {
    using I1to0::I1to0;
    
  public:
    std::string mnemonic() override {
      return "push";
    }
  };
  
  
  // Special case, may be restructured later
  class retq : public Instruction {
    using Instruction::Instruction;
    
  public:
    std::string generate() override {
      return string("retq") + "\t\t\t# " + annotation();
    }
  };
  
  
  // Special case, may be restructured later
  class subq_rsp : public Instruction {
    using Instruction::Instruction;
    
  public:
    unsigned bytes;
    std::string generate() override {
      return "subq $" + std::to_string(bytes) + ", %rsp" + "\t\t\t# " + annotation();
    }
  };
  
  // Special case, may be restructured later
  class addq_rsp : public Instruction {
    using Instruction::Instruction;
    
  public:
    unsigned bytes;
    std::string generate() override {
      return "addq $" + std::to_string(bytes) + ", %rsp" + "\t\t\t# " + annotation();
    }
  };
  
  
  // Special case, may be restructured later
  class mov_from_stack : public Instruction {
    using Instruction::Instruction;
    
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
      return "mov" + suffix + " " + std::to_string(offset) + "(%ebp), " + dest->getRegisterName() + "\t\t\t# " + annotation();
    }
  };
  
  
  // Special case, may be restructured later
  class mov_to_stack : public Instruction {
    using Instruction::Instruction;
    
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
      return "mov" + suffix + " " + src->getRegisterName() + ", "+ std::to_string((signed)offset) + "(%ebp)" + "\t\t\t# " + annotation();
    }
  };
  
  
  // Special case, may be restructured later
  class mov_from_imm : public Instruction {
    using Instruction::Instruction;
    
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
      return "mov" + suffix + " $" + std::to_string(imm_value) + ", " + dest->getRegisterName() + "\t\t\t# " + annotation();
    }
  };
  
  
  class call : public Instruction {
    using Instruction::Instruction;
    
  public:
    Label label;
    shared_ptr<Register> dest; // only for register allocator (will not be printed in assembler)
    
    std::string generate() override {
      return "call " + label + "\t\t\t# " + annotation();;
    }
  };
  
  // static instruction that generates a predefined string
  class StaticInstruction : public Instruction
  {
  public:
    std::string str;
    
    StaticInstruction(std::string str, const char *fnc, int line) : str(str), Instruction(fnc, line) {};

    virtual std::string generate() {
      return str + "\t\t\t# " + annotation();
    };
  };



}
