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
  // create and open output file
  ofstream outputFile("asm.s");
  
  outputFile << ".text\n";
  outputFile << ".globl " << std::string(AMD64LdNamePrefix) << "main\n\n";
  
  for (auto assembler : irgAssembler)
  {
    outputFile << assembler;
  }
  
  outputFile.close();
  
  std::cout << "Created assembler file: asm.s\n";
}

void Backend::runExternalLinker()
{
  // code seperated from creator.cpp to be able to generate output files with better names in future
  
  // --- create runtime library file in working directory ---
  const char * runtimeSource = R"(
#include <stdio.h>

void println(int a)
{
  printf("%d\n", a);
}
  )";
  
  std::ofstream runtimeFile("_runtime.c");
  runtimeFile << runtimeSource;
  runtimeFile.close();
  
  
  // --- link to runtime library and create binary ---
  
  if (system("gcc -o a.out asm.s _runtime.c") != 0)
  {
    throw BackendError("error running linker");
  }
  
  // delete temporary runtime file
  if (system("rm _runtime.c") != 0)
  {
    throw BackendError("assembler file could not be deleted");
  }
  
  std::cout << "Created binary: a.out\n";
  return;
}
