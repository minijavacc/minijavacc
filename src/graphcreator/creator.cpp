#include "creator.h"
#include "typecreator.h"
#include "irbuilder.h"
#include "typecreator.h"

using namespace cmpl;

Creator::Creator(std::shared_ptr<Node> ast) : ast(ast)
{
  ir_init(); // initialize libfirm
  
  // output backend options
  //be_parse_arg("help");
  
  // set instruction set architecture
  if(!be_parse_arg("isa=amd64"))
  {
    std::cerr << "could not set isa=amd64\n";
    return;
  }

  /* currently creates runtime error: 
  #ifdef _WIN32
    be_parse_arg("ia32-gasmode-mingw");
  #elif __APPLE__
    be_parse_arg("ia32-gasmode=acho");
    be_parse_arg("ia32-stackalign=4");
    be_parse_arg("pic");
  #elif __linux__
    be_parse_arg("ia32-gasmode=elf");
  #else
    #error "No backend found for this target platform"
  #endif
  */
}

Creator::~Creator()
{
  ir_finish(); // clean up
}

void Creator::run()
{
  // check for missing return paths
  std::shared_ptr<TypeCreator> t = std::make_shared<TypeCreator>();
  ast->accept(t);
  
  std::shared_ptr<IRBuilder> c = std::make_shared<IRBuilder>();
  ast->accept(c);
}

void Creator::dump()
{
  dump_all_ir_graphs("");
}

void Creator::createAssembler()
{
  // create and open output file
  FILE * output;
  output = fopen("a.s" , "w");
  
  // lowering phase
  //be_lower_for_target();
  
  // run backend
  be_main(output, "input.c");
}

int Creator::linkToRuntimeLibrary()
{
  // link to runtime library
  return system("gcc -o a.out a.s runtime/println.c");
}

