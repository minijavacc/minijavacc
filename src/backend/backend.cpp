#include "backend.h"

using namespace cmpl;


// for use with irg_walk_topological()
void irgNodeWalker(ir_node *node, void *env)
{
  // get this-Pointer
  Backend* _this = static_cast<Backend*>(env);
  
  
}

Backend::Backend(Creator &creator) : creator(creator)
{
  // activate back edges
  size_t irgsNum = get_irp_n_irgs();
  
  // iterate irgs
  for (int i = 0; i < irgsNum; i++)
  {
    ir_graph* irg = get_irp_irg(i);
		edges_activate(irg);
  }
}

Backend::~Backend()
{
  // deactivate back edges
  size_t irgsNum = get_irp_n_irgs();
  
  // iterate irgs
  for (int i = 0; i < irgsNum; i++)
  {
    ir_graph* irg = get_irp_irg(i);
		edges_deactivate(irg);
  }
}

void Backend::run(std::string filepath)
{
  size_t irgsNum = get_irp_n_irgs();
  
  // iterate irgs
  for (int i = 0; i < irgsNum; i++)
  {
    ir_graph *irg = get_irp_irg(i);
    std::vector<std::shared_ptr<Instruction>> instructions;

    instructions = irgSetInstructionOrder(irg);
    irgAllocateRegisters(instructions);
    irgAssembler.push_back(irgCreateInstructions(instructions));
  }
  
  createAssemblerFile(filepath);
  runExternalLinker();
}

void Backend::createAssemblerFile(std::string filepath)
{
  // take std::vector<std::string> irgAssembler and write a complete assembler file to disk
  // try to avoid unneeded string copying
}

void Backend::runExternalLinker()
{
  // use code from Creator() to run linker
  // leave code seperated to be able to generate output files with better names in future
}

std::vector<std::shared_ptr<Instruction>> Backend::irgSetInstructionOrder(ir_graph* irg)
{
  std::vector<std::shared_ptr<Instruction>> instructions;
  
  // walk irg
  irg_walk_topological(irg, irgNodeWalker, (void*) this);
  
  return instructions;
}

void Backend::irgAllocateRegisters(std::vector<std::shared_ptr<Instruction>> &instructions)
{
  // work with instructions-vector
  // 
  // after the allocation only 6(?) registers must be used
}

std::string Backend::irgCreateInstructions(std::vector<std::shared_ptr<Instruction>> &instructions)
{
  return "";
}
