#pragma once

#include "amd64instructions.h"
#include "assemblercontext.h"
#include "registerallocator.h"
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
  
  class GraphAssembler
  {
  public:
    GraphAssembler(ir_graph* irg) : irg(irg) {}
    std::string run();
    ir_graph* irg;
    void irgSerialize();
  
    std::string irgCodeGeneration();
    void phiInsertion();
    
    // methods should be private, but must be called from irgNodeWalker()
    void insertProlog();
    void collectPhi(ir_node *node);
    void buildBlock(ir_node *node);
    void buildConst(ir_node *node);
    void buildCond(ir_node *node);
    void buildJmp(ir_node *node);
    void buildProj(ir_node *node);
    void buildAdd(ir_node *node);
    void buildSub(ir_node *node);
    void buildDiv(ir_node *node);
    void buildMul(ir_node *node);
    void buildMinus(ir_node *node);
    void buildMod(ir_node *node);
    void buildReturn(ir_node *node);
    void buildCall(ir_node *node);
    
  private:
    shared_ptr<map<Label, shared_ptr<LabeledBlock>>> blocks;
    shared_ptr<vector<Label>> labels; // topological order
    
    map<long, shared_ptr<Value>> registers;
    map<long, Label> nodeNrToLabel;
    long topOfStack = 0;
    long nextFreeLabel = 0;
    string labelPrefix;
    shared_ptr<Value> regArgsToValue[6];
    size_t nargs;
    
    RegisterAllocator* registerAllocator;
    shared_ptr<Value> getValue(ir_node *node);
    void setValue(ir_node *node, shared_ptr<Value> r);
    Label getLabel(ir_node *node);
    
    shared_ptr<LabeledBlock> getLabeledBlockForIrNode(ir_node *node);
  };
  
  class GraphAssemblerError : public std::runtime_error
  {
    public:
      GraphAssemblerError(const char* err) : std::runtime_error(err) { }
  };
}
