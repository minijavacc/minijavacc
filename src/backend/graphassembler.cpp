#include "graphassembler.h"
#include <stdio.h>

using namespace cmpl;

int c=0;
// for use with irg_walk_topological()
void irgNodeWalker(ir_node *node, void *env)
{
  // get instructions-Pointer
    std::vector<std::shared_ptr<Instruction>>* instructions = static_cast<std::vector<std::shared_ptr<Instruction>>*>(env);
	//check typ of node
}



std::string GraphAssembler::run()
{
  std::string assemblerOutput;
  std::vector<std::shared_ptr<Instruction>> instructions;

  
  // activate all edges in graph
  edges_activate(irg);

  instructions = irgSerialize();
  irgRegisterAllocation(instructions);
  assemblerOutput = irgCodeGeneration(instructions);
  
  // deactivate edges
  edges_deactivate(irg);
  
  return std::move(assemblerOutput);
}



std::vector<std::shared_ptr<Instruction>> GraphAssembler::irgSerialize()
{
  std::vector<std::shared_ptr<Instruction>> instructions;
  
  // create sequence of instructions (create them on the fly)
  
  // walk irg
  irg_walk_topological(irg, irgNodeWalker, (void*) &instructions);
  
  // Example
  // AddInstr(0, 1, newReg());
  
  return std::move(instructions);
}

void GraphAssembler::irgRegisterAllocation(std::vector<std::shared_ptr<Instruction>> &instructions)
{
  // work with instructions-vector
  // 
  // after the allocation only 6(?) registers must be used
  // save size of needed stack frame somewhere
}

std::string GraphAssembler::irgCodeGeneration(std::vector<std::shared_ptr<Instruction>> &instructions)
{
  // call generate() for every instruction, set labels, create one long string
  // add function prolog/epilog
  
  return std::move(std::string(""));
}

regNum GraphAssembler::newReg()
{
  regNum r = nextRegNum++; // post-increment!
  
  return r;
}
