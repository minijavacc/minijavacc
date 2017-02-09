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
  class Value;
  
  typedef string Label;
  
  // describes one instruction
  class Instruction
  {
  protected:
    string size64suffix = "q";
    string size32suffix = "l";
    
  public:
    int line;
    const char *fnc;
    
    Instruction(const char *fnc, int line) : fnc(fnc), line(line) {}
    virtual string mnemonic();
    virtual string annotation();
    virtual string generate();
  };
  
  class ABtoB : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Value> dest;
    shared_ptr<Value> src1;
    shared_ptr<Value> src2;
    
    string generate() override;
  };
    
  class ABto_ : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Value> src1;
    shared_ptr<Value> src2;
    
    string generate() override;
  };
  
  class Ato_ : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Value> src1;
    
    string generate() override;
  };
  
  class _toA : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Value> dest;
    
    string generate() override;
  };
  
  class AtoA : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Value> src1;
    shared_ptr<Value> dest;
    
    string generate() override;
  };
  
  class AtoB : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Value> src1;
    shared_ptr<Value> dest;
    
    string generate() override;
  };
  
  class cmp : public ABto_ {
    using ABto_::ABto_;
    
    string mnemonic() override;
  };
  
  class Branch : public Instruction {
    using Instruction::Instruction;
    
  public:
    Label label;
    ir_relation relation;
    
    string mnemonic() override;
    string generate() override;
  };
  
  class jmp : public Instruction {
    using Instruction::Instruction;
    
  public:
    Label label;
    
    string generate() override;
  };
  
  class mov : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Value> src1;
    shared_ptr<Value> dest;
    
    mov(const char *fnc, int line) : Instruction(fnc, line) {};
    
    // value from stack
    mov(int offset, shared_ptr<Value> dest, const char *fnc, int line);
    // value to stack
    mov(shared_ptr<Value> src1, int offset, const char *fnc, int line);
    string generate() override;
    string mnemonic() override;
  };
  
  
  class add : public ABtoB {
    using ABtoB::ABtoB;
    
    string mnemonic() override;
  };
  
  class neg : public AtoA {
    using AtoA::AtoA;
    
    string mnemonic() override;
  };
  
  class sub : public ABtoB {
    using ABtoB::ABtoB;
    
    string mnemonic() override;
  };
  
  class div : public Instruction {
    using Instruction::Instruction;
	public:
    shared_ptr<Value> dest;
    shared_ptr<Value> src1;
    shared_ptr<Value> src2;
	
    // result in %eax
    string mnemonic() override;
    string generate() override;
  };
  
  class mod : public Instruction {
    using Instruction::Instruction;
	public:
    shared_ptr<Value> dest;
    shared_ptr<Value> src1;
    shared_ptr<Value> src2;
    
    // result in %edx
    string mnemonic() override;
    string generate() override;
  };
  
  class imul : public ABtoB {
    using ABtoB::ABtoB;
    
    string mnemonic() override;
  };
  
  class pop : public _toA {
    using _toA::_toA;
    
  public:
    string mnemonic() override;
  };

  
  class push : public Ato_ {
    using Ato_::Ato_;
    
  public:
    string mnemonic() override;
  };
  
  class movsxd_rax : public Instruction {
    using Instruction::Instruction;
    
  public:
    shared_ptr<Value> src1;
    string generate() override;
  };
  
  
  // Special case, may be restructured later
  class ret : public Instruction {
    using Instruction::Instruction;
    
  public:
    string generate() override;
  };
  
  
  // Special case, may be restructured later
  class subq_rsp : public Instruction {
    using Instruction::Instruction;
    
  public:
    int bytes;
    string generate() override;
  };
  
  // Special case, may be restructured later
  class addq_rsp : public Instruction {
    using Instruction::Instruction;
    
  public:
    unsigned bytes;
    string generate() override;
  };

  class cltd : public Instruction {
    using Instruction::Instruction;    
  public:
    string generate() override;
  };
  
  
  class call : public Instruction {
    using Instruction::Instruction;
    
  public:
    Label label;
    shared_ptr<Value> dest; // only for register allocator (will not be printed in assembler)
    
    string generate() override;
  };
  
  // static instruction that generates a predefined string
  class StaticInstruction : public Instruction
  {
  public:
    string str;
    
    StaticInstruction(string str, const char *fnc, int line);
    virtual string generate();
  };



}
