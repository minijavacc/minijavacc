//
//  values.h
//  mjcc
//
//  Created by Markus Schlegel on 04/02/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <assert.h>
#include <libfirm/firm.h>
#include "amd64instructions.h"

using namespace std;

namespace cmpl
{
  class StackFrameAllocation;
  
  static int lastUsedRegisterIdentifier = 1000;
  
  enum ValueSize {
    ValueSize64,
    ValueSize32,
    ValueSizeUndefined
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
  
  class Physical;
  
  // Base Value class
  // ------------------
  class Value : public std::enable_shared_from_this<Value> {
  protected:
    ValueSize size;
    
  public:
    virtual string toString();
    virtual ValueSize getSize();
    virtual void setSize(ValueSize s);
    static ValueSize valueSizeFromIRNode(ir_node *node);
    static ValueSize valueSizeFromIRMode(ir_mode *mode);
    virtual shared_ptr<Value> getLowered(shared_ptr<StackFrameAllocation> allocation);
    virtual shared_ptr<mov> movToPhysical(shared_ptr<Physical> p) { assert(false); };
    virtual shared_ptr<mov> movFromPhysical(shared_ptr<Physical> p) { assert(false); };
  };
  
  // Immediate Value class
  // ------------------
  class Immediate : public Value {
  private:
    long immediate;
    
  public:
    Immediate(long i, ir_mode *mode);
    string toString() override;
    virtual shared_ptr<mov> movToPhysical(shared_ptr<Physical> p) override;
  };
  
  // Physical register Value class
  // ------------------
  class Physical : public Value {
  private:
    long identifier;
    
  public:
    Physical(long identifier, ValueSize s);
    string toString() override;
    virtual shared_ptr<mov> movToPhysical(shared_ptr<Physical> p) override;
    virtual shared_ptr<mov> movFromPhysical(shared_ptr<Physical> p) override;
  };
  
  // Memory location Value class
  // ------------------
  class Memory : public Value {
  private:
    shared_ptr<Physical> pointer = nullptr;
    int offset;
    
  public:
    Memory(shared_ptr<Physical> pointer, int offset, ValueSize size);
    string toString() override;
    virtual shared_ptr<mov> movToPhysical(shared_ptr<Physical> p) override;
    virtual shared_ptr<mov> movFromPhysical(shared_ptr<Physical> p) override;
  };
  
  // Virtual register Value class
  // ------------------
  class Virtual : public Value {
  private:
    long identifier;
    
  public:
    Virtual(ValueSize s);
    Virtual(ir_node *node);
    string toString() override;
    shared_ptr<Value> getLowered(shared_ptr<StackFrameAllocation> allocation) override;
  };
}