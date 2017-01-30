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

  class AssemblerContext
  {
  public:

    ir_graph* irg;
    size_t nargs;
   
    map<Label, shared_ptr<LabeledBlock>> blocks;
    vector<Label> labels; // topological order 
    map<long, shared_ptr<Value>> registers;
    map<long, Label> nodeNrToLabel;
    long topOfStack = 0;
    long nextFreeLabel = 0;
    string labelPrefix;
    shared_ptr<Value> regArgsToValue[6];
 };
  

}
