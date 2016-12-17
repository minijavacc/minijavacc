#include "graphassembler.h"
#include <stdio.h>
#include <libfirm/firm.h>


using namespace cmpl;

#pragma mark - Misc

struct ctx {
  std::shared_ptr<std::vector<std::shared_ptr<Instruction>>> instructions;
  std::map<long, long> registers;
  std::map<long, Label> labels;
  long nextFreeRegister;
  long nextFreeLabel;
  std::string labelPrefix;
};

long alloc_new_reg(ctx *x, ir_node *node) {
  long r = x->nextFreeRegister;
  x->registers.emplace(get_irn_node_nr(node), r);
  x->nextFreeRegister = x->nextFreeRegister + 1;
  return r;
}

Label get_label(ctx *x, ir_node *node) {
  if (x->labels.count(get_irn_node_nr(node)) > 0) {
    return x->labels.at(get_irn_node_nr(node));
  }
  
  std::string p = x->labelPrefix;
  Label l = p + std::to_string(x->nextFreeLabel);
  x->labels.emplace(get_irn_node_nr(node), l);
  x->nextFreeLabel = x->nextFreeLabel + 1;
  return l;
}




#pragma mark - Function prolog/epilog

void insertProlog(ctx *x) {
  auto p = std::make_shared<pushq_rbp>();
  auto m = std::make_shared<movq_rsp_rbp>();
  auto s = std::make_shared<subq_rsp>();
  s->nslots = (unsigned) x->nextFreeRegister;
  
  auto it = x->instructions->begin();
  x->instructions->insert(it, p);
  it = x->instructions->begin();
  x->instructions->insert(it + 1, m);
  it = x->instructions->begin();s
  x->instructions->insert(it + 2, s);
}





#pragma mark - Instruction builders

void buildBlock(ir_node *node, ctx *x) {
  ir_graph *g = get_irn_irg(node);
  if (node == get_irg_start_block(g) || node == get_irg_end_block(g)) {
    return;
  }
  
  Label l = get_label(x, node);
  auto lbl = std::make_shared<Lbl>();
  lbl->label = l;
  x->instructions->push_back(lbl);
}

void buildConst(ir_node *node, ctx *x) {
  ir_tarval *val = get_Const_tarval(node);
  long l = get_tarval_long(val);
  auto m = std::make_shared<movl_from_imm>();
  m->imm_value = l;
  long oreg = alloc_new_reg(x, node);
  m->dest = oreg;
  x->instructions->push_back(m);
}

void buildCond(ir_node *node, ctx *x) {
  ir_node *s = get_Cond_selector(node);
  assert(is_Cmp(s));
  
  ir_node *trueBlock = nullptr;
  ir_node *falseBlock = nullptr;
  
  // TODO: there must be a more direct way
  foreach_out_edge_safe(node, edge) {
    ir_node *n = get_edge_src_irn(edge);
    if (is_Proj(n)) {
      int num = get_Proj_num(n);
      ir_node *block = nullptr;
      foreach_out_edge_safe(n, edge_) {
        block = get_edge_src_irn(edge_);
      }
      
      if (num == pn_Cond_true) {
        trueBlock = block;
      }
      
      if (num == pn_Cond_false) {
        falseBlock = block;
      }
    }
  }
  
  assert(trueBlock);
  assert(falseBlock);
  
  Label trueLabel = get_label(x, trueBlock);
  Label falseLabel = get_label(x, falseBlock);
  
  ir_node *l = get_Cmp_left(s);
  ir_node *r = get_Cmp_right(s);
  
  long lreg = x->registers[get_irn_node_nr(l)];
  long rreg = x->registers[get_irn_node_nr(r)];
  
  auto cmp = std::make_shared<cmpl_>();
  cmp->src1 = lreg;
  cmp->src2 = rreg;
  x->instructions->push_back(cmp);
  
  auto br = std::make_shared<Branch>();
  br->relation = get_Cmp_relation(s);
  br->label = trueLabel;
  x->instructions->push_back(br);
  
  auto j = std::make_shared<jmp>();
  j->label = falseLabel;
  x->instructions->push_back(j);
}

void buildProj(ir_node *node, ctx *x) {
  ir_node *pred = get_Proj_pred(node);
  if (is_Proj(pred)) {
    ir_node *ppred = get_Proj_pred(pred);
    if (is_Start(ppred)) {
      // Is an argument
      auto m = std::make_shared<movl_from_stack>();
      m->offset = get_Proj_num(node);
      long oreg = alloc_new_reg(x, node);
      m->dest = oreg;
      x->instructions->push_back(m);
    }
  }
}

void buildAdd(ir_node *node, ctx *x) {
  ir_node *l = get_Add_left(node);
  ir_node *r = get_Add_right(node);
  
  long lreg = x->registers[get_irn_node_nr(l)];
  long rreg = x->registers[get_irn_node_nr(r)];
  long oreg = alloc_new_reg(x, node);
  
  auto inst = std::make_shared<addl>();
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  x->instructions->push_back(inst);
}

void buildReturn(ir_node *node, ctx *x) {
  if (get_Return_n_ress(node) > 0) {
    ir_node *pred = get_Return_res(node, 0);
    long r = x->registers[get_irn_node_nr(pred)];
    auto inst = std::make_shared<movl_to_rax>();
    inst->src1 = r;
    x->instructions->push_back(inst);
  }
  
  auto pop = std::make_shared<popq_rbp>();
  x->instructions->push_back(pop);
  auto ret = std::make_shared<retq>();
  x->instructions->push_back(ret);
}


int nodeNum=0;
// for use with irg_walk_topological()
void irgNodeWalker(ir_node *node, void *env)
{
  
  ctx *x = (struct ctx *) env;
  
  if (is_Block(node)) {
    buildBlock(node, x);
  }
  
  if (is_Const(node)) {
    buildConst(node, x);
  }
      
  if (is_Cond(node)) {
    buildCond(node, x);
  }
  
  if (is_Proj(node)) {
    buildProj(node, x);
  }
    
  if (is_Add(node)) {
    buildAdd(node, x);
  }
    
  if (is_Return(node)) {
    buildReturn(node, x);
  }

  
  
  
  
//	nodeNum++;
//  // get instructions-Pointer
//    std::vector<std::shared_ptr<Instruction>>* instructions = static_cast<std::vector<std::shared_ptr<Instruction>>*>(env);
//	//check typ of node
//	printf("> Node Number: %d \n",nodeNum);
//	if(is_Block(node))
//	{
//		//check if its the first block
//		if(get_irn_arity(node)==0)
//		{
//			//get the methods parameter and return
//			ir_entity* entity=get_irg_entity(get_irn_irg(node));
//			const char* name=get_entity_name(entity); 
//			ir_type* method=get_entity_type(entity);
//			size_t paramsSize=get_method_n_params(method);
//			size_t returnSize=get_method_n_ress(method);
//			//handle return type
//			if(returnSize==1)
//			{
//				ir_type* returnType=get_method_res_type(method,0);
//				if(is_Primitive_type(returnType))
//				{
//					ir_mode* mode=get_type_mode(returnType);
//					if(mode==mode_Is)
//					{
//						//int
//					}
//					else if(mode==mode_Bu){
//						//bool
//					}
//					else {
//						//only int and bool primitives are valid
//						assert(false);
//					}
//				}
//				else
//				{
//					//is pointer-type for usertypes or arrays
//					
//				}
//			}
//			
//			
//			printf("method: name = %s  paramCount = %d  returnCount = %d \n",name,paramsSize,returnSize);
//			
//		}	
//	}
//	else if(is_Add(node))
//	{
//		printf("node is add \n");		
//	}
//	else if(is_Const(node))
//	{
//		long l=get_tarval_long(get_Const_tarval(node));
//		printf("node is const %ld \n",l);
//		//regNum num=newReg();
//	}
}





#pragma mark - Methods

std::string GraphAssembler::run()
{
  // Store number of arguments
  std::string assemblerOutput;
  instructions = std::make_shared<std::vector<std::shared_ptr<Instruction>>>();
  ir_entity *ent = get_irg_entity(irg);
  ir_type *ty = get_entity_type(ent);
  nargs = get_method_n_params(ty);

  
  // activate all edges in graph
  edges_activate(irg);
  
  irgSerialize();
  irgRegisterAllocation();
  assemblerOutput = irgCodeGeneration();
  
  // deactivate edges
  edges_deactivate(irg);
  
  return std::move(assemblerOutput);
}



void GraphAssembler::irgSerialize()
{
  // Initialize context
  ctx x;
  x.nextFreeRegister = 0;
  x.nextFreeLabel = 0;
  x.labelPrefix = "L_";
  x.instructions = instructions;
  
  // Walk graph
  irg_walk_topological(irg, irgNodeWalker, &x);
  
  // Insert prolog
  insertProlog(&x);
  
  
//  for (auto const& i : *x.instructions) {
//    std::cout << i->generate() << "\n";
//  }
//  
//  std::cout << "\n\n";
}

void GraphAssembler::irgRegisterAllocation()
{
  auto is = std::make_shared<std::vector<std::shared_ptr<Instruction>>>();
  
  // work with instructions-vector
  // primitive: just use 2 registers
  for(auto& instruction: *instructions) {
    if (auto i = dynamic_cast<I2to1*>(instruction.get())) {
      // load arg1
      auto m1 = std::make_shared<movl_from_stack>();
      m1->offset = (unsigned) (i->src1 + nargs);
      m1->dest = 0;
      
      // load arg2
      auto m2 = std::make_shared<movl_from_stack>();
      m2->offset = (unsigned) (i->src2 + nargs);
      m2->dest = 1;
      
      i->src1 = m1->dest;
      i->src2 = m2->dest;
      i->dest = i->src2;
      
      is->push_back(m1);
      is->push_back(m2);
      is->push_back(instruction);
      continue;
    }
    
    if (auto i = dynamic_cast<I2to0*>(instruction.get())) {
      // load arg1
      auto m1 = std::make_shared<movl_from_stack>();
      m1->offset = (unsigned) (i->src1 + nargs);
      m1->dest = 0;
      
      // load arg2
      auto m2 = std::make_shared<movl_from_stack>();
      m2->offset = (unsigned) (i->src2 + nargs);
      m2->dest = 1;
      
      i->src1 = m1->dest;
      i->src2 = m2->dest;
      
      is->push_back(m1);
      is->push_back(m2);
      is->push_back(instruction);
      continue;
    }
    
    if (auto i = dynamic_cast<I1to0*>(instruction.get())) {
      // load arg1
      auto m1 = std::make_shared<movl_from_stack>();
      m1->offset = (unsigned) (i->src1 + nargs);
      m1->dest = 0;
      
      i->src1 = m1->dest;
      
      is->push_back(m1);
      is->push_back(instruction);
      continue;
    }
    
    if (auto i = dynamic_cast<movl_from_stack*>(instruction.get())) {
      // save result to stack
      auto m = std::make_shared<movl_to_stack>();
      m->offset = (unsigned) (i->dest + nargs);
      i->dest = 0;
      m->src = i->dest;
      
      is->push_back(instruction);
      is->push_back(m);
      continue;
    }
    
    if (auto i = dynamic_cast<movl_from_imm*>(instruction.get())) {
      // save result to stack
      auto m = std::make_shared<movl_to_stack>();
      m->offset = (unsigned) (i->dest + nargs);
      i->dest = 0;
      m->src = i->dest;
      
      is->push_back(instruction);
      is->push_back(m);
      continue;
    }
    
    is->push_back(instruction);
  }
  
  instructions = is;
  
  for (auto const& i : *instructions) {
    std::cout << i->generate() << "\n";
  }
  
  std::cout << "\n\n";
  
  // enhanced: from https://en.wikipedia.org/wiki/X86_calling_conventions#x86-64_calling_conventions
  // System V AMD64: 6 times int/pointer {RDI, RSI, RDX, RCX, R8, R9} ["certain" floating point {XMM0–7} probably irrelevant for us] Caller Stack aligned on 16 bytes
  // after the allocation only 6 registers must be used
  // save size of needed stack frame somewhere
}

std::string GraphAssembler::irgCodeGeneration()
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
