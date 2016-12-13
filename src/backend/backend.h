#pragma once

#include "amd64instructions.h"
#include "creator.h"

#include <libfirm/firm.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <memory>

namespace cmpl
{

  class Backend
  {
  public:
    Backend(Creator &creator);
    ~Backend();
    
    void run(std::string filepath);
    
    std::vector<std::shared_ptr<Instruction>> irgSetInstructionOrder(ir_graph* irg);
    void irgAllocateRegisters(std::vector<std::shared_ptr<Instruction>> &instructions);
    std::string irgCreateInstructions(std::vector<std::shared_ptr<Instruction>> &instructions);
    
    void createAssemblerFile(std::string filepath);
    void runExternalLinker();
    
  private:
    Creator &creator;
    
    // list of the assembler output of all graphs (functions)
    std::vector<std::string> irgAssembler;
  };
  
  class BackendError : public std::runtime_error
  {
    public:
      BackendError(const char* err) : std::runtime_error(err) { }
  };

  // for use with irg_walk_topological()
  void irgNodeWalker(ir_node *node, void* env);
}
