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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void println(int32_t a) {
/* The ISO C standard requires a long int to have at least 32 bits. */
  if (fprintf(stdout , "%ld\n", (long) x) < 0) {
    fprintf(stderr , "error:␣println:␣%s\n", strerror(errno));
    abort();
  }
  //printf("%d\n", a);
}

void write(const int32_t b) {
  /* Cast the argument back and forth because an int may only provide 16 bits
     of precision and signed overflow is undefined behavior in ISO C. */
  const int octet = (int) (((unsigned) b) & 0xffU);
  if (fputc(octet , stdout) < 0) {
    fprintf(stderr , "error:␣write:␣%s\n", strerror(errno));
    abort();
  }
}

void flush() {
  if (fflush(stdout) < 0) {
    fprintf(stderr , "error:␣flush:␣%s\n", strerror(errno));
    abort();
  }
}

int32_t read() {
  const int c = fgetc(stdin);
  if ((c < 0) && ferror(stdin)) {
    fprintf(stderr , "error:␣read:␣%s\n", strerror(errno));
    abort();
  }
  return (int32_t) c;
}

void exit(const int32_t status) {
  exit((int) status);
}

/*
void println(int a)
{
  printf("%d\n", a);
}

void write(int a)
{
  printf("%c", a);
}

void flush()
{
  printf("\n");
}

int read()
{
  int x;
  scanf("%d", &x);
  return x;
}

void exit(int status)
{
  exit(status);
}*/
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
