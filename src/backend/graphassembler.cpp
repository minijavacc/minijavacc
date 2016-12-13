#include "graphassembler.h"


using namespace cmpl;


// for use with irg_walk_topological()
void irgNodeWalker(ir_node *node, void *env)
{
  // get this-Pointer
  GraphAssembler* _this = static_cast<GraphAssembler*>(env);
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
  irg_walk_topological(irg, irgNodeWalker, (void*) this);
  
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
