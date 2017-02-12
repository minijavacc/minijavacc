//
//  delivervalue-tests.h
//  mjcc
//
//  Created by Markus Schlegel on 02/02/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#pragma once

#include <vector>
#include <memory>
#include "amd64instructions.h"
#include "assemblercontext.h"
#include "registerallocator.h"

namespace cmpl
{
  using namespace std;
  
  class DeliverValueTests
  {
  public:
    vector<shared_ptr<Instruction>> instructions;
    void run();
    void init();
    
    RegisterAllocator *registerAllocator;
    shared_ptr<Value> valVirtual32;
    shared_ptr<Value> valVirtual64;
    shared_ptr<Value> valImm32;
    shared_ptr<Value> valImm64;
    shared_ptr<Value> valStackSlot32;
    shared_ptr<Value> valStackSlot64;
    shared_ptr<Value> valGenImm32;
    shared_ptr<Value> valGenImm64;
    
//    void v32_v32();
//    void v32_v64();
//    void v32_p32();
//    void v32_p64();
//    void v32_i32();
//    void v32_i64();
//    void v32_s32();
//    void v32_s64();
  };
}