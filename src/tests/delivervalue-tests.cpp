//
//  delivervalue-tests.cpp
//  mjcc
//
//  Created by Markus Schlegel on 02/02/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#include "delivervalue-tests.h"
#include "amd64instructions.h"
#include <libfirm/firm.h>

using namespace cmpl;

void DeliverValueTests::init() {
  auto blocks = make_shared<map<Label, shared_ptr<LabeledBlock>>>();
  auto labels = make_shared<vector<Label>>();
  registerAllocator = new RegisterAllocator(blocks, labels, 0);
  
  instructions.clear();
  
//  valVirtual32 = make_shared<Value>(ValueSize32);
//  valVirtual64 = make_shared<Value>(ValueSize64);
//  
//  valImm32 = make_shared<Value>();
//  valImm32->size = ValueSize32;
//  valImm32->type = ValueTypeImmediate;
//  valImm32->immediate = 23;
//  
//  valImm64 = make_shared<Value>();
//  valImm64->size = ValueSize64;
//  valImm64->type = ValueTypeImmediate;
//  valImm64->immediate = 42;
//  
//  valStackSlot32 = make_shared<Value>(ValueSize32, 99);
//  valStackSlot64 = make_shared<Value>(ValueSize64, 1337);
//  valGenImm32 = make_shared<Value>(7, ValueSize32);
//  valGenImm64 = make_shared<Value>(11, ValueSize64);
}

void DeliverValueTests::run() {
//  init();
//  v32_v32();
//  
//  init();
//  v32_v64();
//  
//  init();
//  v32_p32();
//  
//  init();
//  v32_p64();
//  
//  init();
//  v32_i32();
//  
//  init();
//  v32_i64();
//  
//  init();
//  v32_s32();
//  
//  init();
//  v32_s64();
}

//void DeliverValueTests::v32_v32() {
//  registerAllocator->deliverValue(valVirtual32, valVirtual32, instructions);
//}
