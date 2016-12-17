#include "graphassembler.h"
#include <stdio.h>
#include <vector>
#include <libfirm/firm.h>


using namespace cmpl;

#pragma mark - Misc

struct ctx {
  shared_ptr<map<Label, shared_ptr<vector<shared_ptr<Instruction>>>>> blocks;
  shared_ptr<vector<Label>> labels;
  map<long, long> registers;
  map<long, Label> nodeNrToLabel;
  long nextFreeRegister;
  long nextFreeLabel;
  string labelPrefix;
};

long alloc_new_reg(ctx *x, ir_node *node) {
  long r = x->nextFreeRegister;
  x->registers.emplace(get_irn_node_nr(node), r);
  x->nextFreeRegister = x->nextFreeRegister + 1;
  return r;
}

Label get_label(ctx *x, ir_node *node) {
  if (x->nodeNrToLabel.count(get_irn_node_nr(node)) > 0) {
    return x->nodeNrToLabel.at(get_irn_node_nr(node));
  }
  
  string p = x->labelPrefix;
  Label l = p + to_string(x->nextFreeLabel);
  x->nodeNrToLabel.emplace(get_irn_node_nr(node), l);
  x->nextFreeLabel = x->nextFreeLabel + 1;
  return l;
}

shared_ptr<vector<shared_ptr<Instruction>>> get_current_block(ctx *x) {
//  auto curlabel = x->labels->back();
//  auto curblock = x->blocks->at(curlabel);
  return x->blocks->at(x->labels->back());
}




#pragma mark - Function prolog/epilog

void insertProlog(ctx *x) {
  auto p = make_shared<pushq_rbp>();
  auto m = make_shared<movq_rsp_rbp>();
  auto s = make_shared<subq_rsp>();
  s->nslots = (unsigned) x->nextFreeRegister;
  
  Label fl = x->labels->front();
  auto is = x->blocks->at(fl);
  auto it = is->begin();
  x->blocks->at(fl)->insert(it, p);
  it = is->begin();
  is->insert(it + 1, m);
  it = is->begin();
  is->insert(it + 2, s);
}





#pragma mark - Instruction builders

void buildBlock(ir_node *node, ctx *x) {
  ir_graph *g = get_irn_irg(node);
  if (node == get_irg_end_block(g)) {
    return;
  }
  
  Label l = get_label(x, node);
  auto bl = make_shared<vector<shared_ptr<Instruction>>>();
  x->blocks->emplace(l, bl);
  x->labels->push_back(l);
}

void buildConst(ir_node *node, ctx *x) {
  ir_tarval *val = get_Const_tarval(node);
  long l = get_tarval_long(val);
  auto m = make_shared<movl_from_imm>();
  m->imm_value = l;
  long oreg = alloc_new_reg(x, node);
  m->dest = oreg;
  
  get_current_block(x)->push_back(m);
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
  
  auto cmp = make_shared<cmpl_>();
  cmp->src1 = lreg;
  cmp->src2 = rreg;
  get_current_block(x)->push_back(cmp);
  
  auto br = make_shared<Branch>();
  br->relation = get_Cmp_relation(s);
  br->label = trueLabel;
  get_current_block(x)->push_back(br);
  
  auto j = make_shared<jmp>();
  j->label = falseLabel;
  get_current_block(x)->push_back(j);
}

void buildJmp(ir_node *node, ctx *x) {
  foreach_out_edge_safe(node, edge) {
    ir_node *n = get_edge_src_irn(edge);
    assert(is_Block(n));
    Label l = get_label(x, n);
    
    auto j = make_shared<jmp>();
    j->label = l;
    get_current_block(x)->push_back(j);
  }
}

void buildProj(ir_node *node, ctx *x) {
  ir_node *pred = get_Proj_pred(node);
  if (is_Proj(pred)) {
    ir_node *ppred = get_Proj_pred(pred);
    if (is_Start(ppred)) {
      // Is an argument
      auto m = make_shared<movl_from_stack>();
      m->offset = get_Proj_num(node);
      long oreg = alloc_new_reg(x, node);
      m->dest = oreg;
      get_current_block(x)->push_back(m);
    }
  }
}

void buildAdd(ir_node *node, ctx *x) {
  ir_node *l = get_Add_left(node);
  ir_node *r = get_Add_right(node);
  
  long lreg = x->registers[get_irn_node_nr(l)];
  long rreg = x->registers[get_irn_node_nr(r)];
  long oreg = alloc_new_reg(x, node);
  
  auto inst = make_shared<addl>();
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  get_current_block(x)->push_back(inst);
}

void buildReturn(ir_node *node, ctx *x) {
  if (get_Return_n_ress(node) > 0) {
    ir_node *pred = get_Return_res(node, 0);
    long r = x->registers[get_irn_node_nr(pred)];
    auto inst = make_shared<movl_to_rax>();
    inst->src1 = r;
    get_current_block(x)->push_back(inst);
  }
  
  auto pop = make_shared<popq_rbp>();
  get_current_block(x)->push_back(pop);
  auto ret = make_shared<retq>();
  get_current_block(x)->push_back(ret);
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
  
  if (is_Jmp(node)) {
    buildJmp(node, x);
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
}





#pragma mark - Methods

string GraphAssembler::run()
{
  // Store number of arguments
  string assemblerOutput;
  blocks = make_shared<map<Label, shared_ptr<vector<shared_ptr<Instruction>>>>>();
  labels = make_shared<vector<Label>>();
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
  
  return move(assemblerOutput);
}



void GraphAssembler::irgSerialize()
{
  // Initialize context
  ctx x;
  x.nextFreeRegister = 0;
  x.nextFreeLabel = 0;
  x.labelPrefix = "L_";
  x.labels = labels;
  x.blocks = blocks;
  
  // Walk graph
  irg_walk_topological(irg, irgNodeWalker, &x);
  
  // Insert prolog
  insertProlog(&x);
  
  
//  for (auto const& i : *x.instructions) {
//    cout << i->generate() << "\n";
//  }
//  
//  cout << "\n\n";
}

void GraphAssembler::irgRegisterAllocation()
{
  // work with instructions-vector
  // primitive: just use 2 registers
  for (auto const& label : *labels) {
    auto instructions = blocks->at(label);
    auto instructions_ = make_shared<vector<shared_ptr<Instruction>>>();
    
    for (auto const& instruction : *instructions) {
      if (auto i = dynamic_cast<I2to1*>(instruction.get())) {
        // load arg1
        auto m1 = make_shared<movl_from_stack>();
        m1->offset = (unsigned) (i->src1 + nargs);
        m1->dest = 0;

        // load arg2
        auto m2 = make_shared<movl_from_stack>();
        m2->offset = (unsigned) (i->src2 + nargs);
        m2->dest = 1;

        i->src1 = m1->dest;
        i->src2 = m2->dest;
        i->dest = i->src2;
        
        instructions_->push_back(m1);
        instructions_->push_back(m2);
        instructions_->push_back(instruction);
        continue;
      }
      
      if (auto i = dynamic_cast<I1to0*>(instruction.get())) {
        // load arg1
        auto m1 = make_shared<movl_from_stack>();
        m1->offset = (unsigned) (i->src1 + nargs);
        m1->dest = 0;

        i->src1 = m1->dest;

        instructions_->push_back(m1);
        instructions_->push_back(instruction);
        continue;
      }

      if (auto i = dynamic_cast<movl_from_stack*>(instruction.get())) {
        // save result to stack
        auto m = make_shared<movl_to_stack>();
        m->offset = (unsigned) (i->dest + nargs);
        i->dest = 0;
        m->src = i->dest;

        instructions_->push_back(instruction);
        instructions_->push_back(m);
        continue;
      }

      if (auto i = dynamic_cast<movl_from_imm*>(instruction.get())) {
        // save result to stack
        auto m = make_shared<movl_to_stack>();
        m->offset = (unsigned) (i->dest + nargs);
        i->dest = 0;
        m->src = i->dest;
        
        instructions_->push_back(instruction);
        instructions_->push_back(m);
        continue;
      }
      
      instructions_->push_back(instruction);
    }
    
    blocks->emplace(label, instructions_);
  }
  
  
  
  for (auto const& label : *labels) {
    cout << label << ":\n";
    
    auto instructions = blocks->at(label);
    
    for (auto const& instruction : *instructions) {
      cout << "\t" + instruction->generate() << "\n";
    }
  }
  
  
  
  // enhanced: from https://en.wikipedia.org/wiki/X86_calling_conventions#x86-64_calling_conventions
  // System V AMD64: 6 times int/pointer {RDI, RSI, RDX, RCX, R8, R9} ["certain" floating point {XMM0–7} probably irrelevant for us] Caller Stack aligned on 16 bytes
  // after the allocation only 6 registers must be used
  // save size of needed stack frame somewhere
}

string GraphAssembler::irgCodeGeneration()
{
  // call generate() for every instruction, set labels, create one long string
  // add function prolog/epilog
  
  return move(string(""));
}

regNum GraphAssembler::newReg()
{
  regNum r = nextRegNum++; // post-increment!
  
  return r;
}
