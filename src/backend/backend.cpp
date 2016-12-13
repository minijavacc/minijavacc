#include "backend.h"

using namespace cmpl;


void Backend::run(std::string filepath)
{
  size_t irgsNum = get_irp_n_irgs();
  
  // iterate irgs
  for (int i = 0; i < irgsNum; i++)
  {
    ir_graph *irg = get_irp_irg(i);
    
    GraphAssembler ga(irg);
    irgAssembler.push_back(ga.run());
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
