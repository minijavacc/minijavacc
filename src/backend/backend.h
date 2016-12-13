#pragma once

#include "amd64instructions.h"
#include "graphassembler.h"
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
    Backend(Creator &creator) : creator(creator) {}
    
    void run(std::string filepath);
    
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
}
