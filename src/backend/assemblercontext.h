#pragma once

#include "amd64instructions.h"

#include <libfirm/firm.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <memory>
#include <map>

namespace cmpl
{
  using namespace std;
  
  class Virtual;
  
  class LabeledBlock {
  public:
    Label label;
    vector<shared_ptr<Instruction>> instructions;
    vector<ir_node *> phis;
    vector<shared_ptr<Instruction>> exitInstructions;
    
    void finalize();
  };
  
  class StackFrameAllocation {
  private:
    int topOfStack = 0;
    
  public:
    map<shared_ptr<Value>, int> allocations;
    
    int getTopOfStack();
    int allocate(shared_ptr<Value> v);
  };

}
