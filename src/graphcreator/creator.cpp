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
  
  // set instruction set architecture
  // has to be set before the graph is created
  if(!be_parse_arg("isa=amd64"))
  {
    throw CreatorBackendError("could not set isa=amd64");
  }

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

void Creator::dumpGraph()
{
  dump_all_ir_graphs("");
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


void Creator::createBinary(std::string filepath)
{
  // get filename without extension
  std::string filename;
  size_t lastindex = filepath.find_last_of(".");
  if (lastindex == std::string::npos)
  {
    filename = filepath + "_";
  }
  else
  {
    filename = filepath.substr(0, lastindex); 
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
  be_lower_for_target();
  
  
  // --- run backend to create assembler ---
  
  // create and open output file
  FILE * output;
  output = fopen((filename + ".s").c_str() , "w");
  
  // run backend to generate assembler file
  be_main(output, (filename + ".c").c_str());
  fclose(output);
  
  std::cout << "Created assembler file: " << filename << ".s\n";
  
  
  // --- link to runtime library and create binary ---
  
  pid_t pid;
  if (pid = fork())
  {
    // parent
    int status;
    waitpid(pid, &status, 0);
    
    if (status != 0)
    {
      throw CreatorBackendError(("could not link assembler file to runtime library. Try running the link command manually:\n$ gcc -o " + filename + " " + filename + ".s runtime/println.c").c_str());
    }
  }
  else
  {
    // child
    int ret = execlp("gcc", "gcc", "-o", filename.c_str(), (filename + ".s").c_str(), "runtime/println.c", NULL);
    
    if (ret != 0)
    {
      throw CreatorBackendError("error running linker");
    }
  }
  
  std::cout << "Created binary: " << filename << "\n";
  return;
}

