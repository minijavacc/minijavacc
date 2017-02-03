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
  
  class LabeledBlock {
  public:
    Label label;
    vector<shared_ptr<Instruction>> instructions;
    vector<ir_node *> phis;
    vector<shared_ptr<Instruction>> exitInstructions;
    
    LabeledBlock() {
      instructions = vector<shared_ptr<Instruction>>();
    }
    
    void finalize() {
      for (auto inst : exitInstructions) {
        instructions.push_back(inst);
      }
    }
  };

}
