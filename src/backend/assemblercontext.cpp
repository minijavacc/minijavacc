//
//  assemblercontext.cpp
//  mjcc
//
//  Created by Markus Schlegel on 05/02/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#include "assemblercontext.h"
#include "values.h"


namespace cmpl {
 
  void LabeledBlock::finalize() {
    for (auto inst : exitInstructions) {
      instructions.push_back(inst);
    }
  }
    

  int StackFrameAllocation::getTopOfStack() {
    return topOfStack;
  }
    
  int StackFrameAllocation::allocate(shared_ptr<Value> v) {
    topOfStack -= 8;
    allocations.emplace(v, topOfStack);
    return topOfStack;
  }
  
}
