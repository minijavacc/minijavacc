#include "graphassembler.h"
#include <stdio.h>
#include <libfirm/firm.h>

using namespace cmpl;

int nodeNum=0;
// for use with irg_walk_topological()
void irgNodeWalker(ir_node *node, void *env)
{
	nodeNum++;
  // get instructions-Pointer
    std::vector<std::shared_ptr<Instruction>>* instructions = static_cast<std::vector<std::shared_ptr<Instruction>>*>(env);
	//check typ of node
	printf("> Node Number: %d \n",nodeNum);
	if(is_Block(node))
	{
		//check if its the first block
		if(get_irn_arity(node)==0)
		{
			ir_entity* entity=get_irg_entity(get_irn_irg(node));
			const char* name=get_entity_name(entity); 
			ir_type* type=get_entity_type(entity);
			size_t paramsSize=get_method_n_params(type);
			size_t returnSize=get_method_n_ress(type);
			
			printf("method: name = %s  paramCount = %d  returnCount = %d \n",name,paramsSize,returnSize);
			
		}	
	}
	else if(is_Add(node))
	{
		printf("node is add \n");		
	}
	else if(is_Const(node))
	{
		long l=get_tarval_long(get_Const_tarval(node));
		printf("node is const %ld \n",l);
		//regNum num=newReg();
	}
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
  irg_walk_topological(irg, irgNodeWalker, (void*) &instructions);
  
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
