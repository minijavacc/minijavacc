#include "creator.h"
#include "irinitializer.h"
#include "irbuilder.h"

#include <sys/wait.h>
#include <unistd.h>

using namespace cmpl;

Creator::Creator(Checker &checker) : checker(checker), ast(checker.getAttributedAST())
{
  ir_init(); // initialize libfirm
  
  set_optimize(0); // deactivate local optimizing
  
  // output backend options
  //be_parse_arg("help");
  
  // set mode_P to P64
  mode_P64 = new_reference_mode("P64", irma_twos_complement, 64, 64);
  set_modeP(mode_P64);
  
  // set instruction set architecture
  // has to be set before the graph is created
  if(!be_parse_arg("isa=amd64"))
  {
    throw CreatorBackendError("could not set isa=amd64");
  }
  
  #ifdef _WIN32
    be_parse_arg("ia32-gasmode-mingw");
  #elif __APPLE__
    be_parse_arg("objectformat=mach-o");
  #elif __linux__
    be_parse_arg("ia32-gasmode=elf");
  #else
    #error "No backend found for this target platform"
  #endif
}

Creator::~Creator()
{
  ir_finish(); // clean up
}

void Creator::run()
{
  // check for missing return paths
  std::shared_ptr<IRInitializer> t = std::make_shared<IRInitializer>();
  ast->accept(t);
  
  std::shared_ptr<IRBuilder> c = std::make_shared<IRBuilder>();
  ast->accept(c);
}

void Creator::dumpGraphs(std::string suffix)
{
  dump_all_ir_graphs(suffix.c_str());
}

/* inline functions from libfirm/ir/tr/type_t.h that get not 
 * included via #include <libfirm/firm.h> */
static inline int is_class_type_(const ir_type *type)
{
	return get_type_opcode(type) == tpo_class;
}

static inline int is_method_type_(ir_type const *const type)
{
	return get_type_opcode(type) == tpo_method;
}


void Creator::createBinary(std::string filepath, bool generateDebugInformation)
{
  // --- validate graphs ---
  //irg_verify(irg);
  
  if (generateDebugInformation)
  {
    be_parse_arg("debug=basic");
  }
  
  
  // --- lowering phase ---
  
  // 1. layout types
  size_t typesNum = get_irp_n_types();
  
  // iterate types
  for (int i = 0; i < typesNum; i++)
  {
    const ir_type* type = get_irp_type(i);
    
    // check if type is a class
    if (is_class_type_(type))
    {
      // iterate members of this class
      size_t membersNum = get_compound_n_members(type);
      
      for (int j = 0; j < membersNum; j++)
      {
        ir_entity* member = get_compound_member(type, j);
        const ir_type* memberType = get_entity_type(member);
        
        if (is_method_type_(memberType))
        {
          set_entity_owner(member, get_glob_type());
          
          // the for-loop has to be adapted because we removed an element
          membersNum--;
          j--;
        }
      }
    }
  }
  
  // 2. lower SELs
  lower_highlevel();
  be_lower_for_target(); 
  
  // --- run backend to create assembler ---
  
  // create and open output file
  FILE * output;
  output = fopen("asm.s" , "w");
  
  // run backend to generate assembler file
  be_main(output, filepath.c_str());
  fclose(output);
  
  std::cout << "Created assembler file: asm.s\n";
  
  
  // --- create runtime library file in working directory ---
  const char * runtimeSource = R"(
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void println(int32_t a) {
/* The ISO C standard requires a long int to have at least 32 bits. */
  if (fprintf(stdout , "%ld\n", (long) a) < 0) {
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
    throw CreatorBackendError("error running linker");
  }
  
  // delete temporary runtime file
  if (system("rm _runtime.c") != 0)
  {
    throw CreatorBackendError("assembler file could not be deleted");
  }
  
  std::cout << "Created binary: a.out\n";
  return;
}

