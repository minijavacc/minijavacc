#include "graphassembler.h"
#include <stdio.h>
#include <vector>
#include <libfirm/firm.h>


using namespace cmpl;

#pragma mark - Misc

long GraphAssembler::allocateReg(ir_node *node) {
  long r = nextFreeRegister;
  registers.emplace(get_irn_node_nr(node), r);
  nextFreeRegister = nextFreeRegister + 1;
  return r;
}

Label GraphAssembler::getLabel(ir_node *node) {
  if (nodeNrToLabel.count(get_irn_node_nr(node)) > 0) {
    return nodeNrToLabel.at(get_irn_node_nr(node));
  }
  
  // if is the first label, take the function name
  if (nodeNrToLabel.empty())
  {
    Label l = get_entity_ld_name(get_irg_entity(irg));
    nodeNrToLabel.emplace(get_irn_node_nr(node), l);
    return l;
  }
  else
  {
    string p = labelPrefix;
    Label l = p + to_string(nextFreeLabel);
    nodeNrToLabel.emplace(get_irn_node_nr(node), l);
    nextFreeLabel = nextFreeLabel + 1;
    return l;
  }
}

shared_ptr<LabeledBlock> GraphAssembler::getCurrentBlock() {
  return blocks->at(labels->back());
}




#pragma mark - Function prolog/epilog

void GraphAssembler::insertProlog() {
  auto p = make_shared<pushq_rbp>();
  auto m = make_shared<movq_rsp_rbp>();
  auto s = make_shared<subq_rsp>();
  s->nslots = (unsigned) nextFreeRegister;
  
  Label fl = labels->front();
  auto bl = blocks->at(fl);
  auto it = bl->instructions->begin();
  bl->instructions->insert(it, p);
  it = bl->instructions->begin();
  bl->instructions->insert(it + 1, m);
  it = bl->instructions->begin();
  bl->instructions->insert(it + 2, s);

  // rename first label to function name
}

// Epilog is implicit in buildReturn()
// move to single location to avoid code duplication?




#pragma mark - Instruction builders

void GraphAssembler::handlePhi(ir_node *node) {
  regNum outReg = allocateReg(node);
  ir_node *bl = get_nodes_block(node);
  for (int i = 0; i < get_Phi_n_preds(node); i++) {
    ir_node *phipred = get_Phi_pred(node, i);
    regNum inReg = registers.at(get_irn_node_nr(phipred));
    
    ir_node *j = get_Block_cfgpred(bl, i);
    ir_node *jbl = get_nodes_block(j);
    
    Label l = nodeNrToLabel.at(get_irn_node_nr(jbl));
    shared_ptr<LabeledBlock> lb = blocks->at(l);
    
    auto m = make_shared<movl>();
    m->src1 = inReg;
    m->dest = outReg;
    
    for (int i_ = 0; i_ < lb->instructions->size(); i_++) {
      if ((*(lb->instructions))[i_] == lb->exitInstruction) {
        auto it = lb->instructions->begin();
        lb->instructions->insert(it + i_, m);
        break;
      }
    }
  }
}

void GraphAssembler::buildBlock(ir_node *node) {
  ir_graph *g = get_irn_irg(node);
  if (node == get_irg_end_block(g)) {
    return;
  }
  
  Label l = getLabel(node);
  auto lb = make_shared<LabeledBlock>();
  lb->label = l;
  blocks->emplace(l, lb);
  labels->push_back(l);
}

void GraphAssembler::buildConst(ir_node *node) {
  ir_tarval *val = get_Const_tarval(node);
  long l = get_tarval_long(val);
  auto m = make_shared<movl_from_imm>();
  m->imm_value = l;
  long oreg = allocateReg(node);
  m->dest = oreg;
  
  getCurrentBlock()->instructions->push_back(m);
}

void GraphAssembler::buildCond(ir_node *node) {
  ir_node *s = get_Cond_selector(node);
  assert(is_Cmp(s));
  
  ir_node *trueBlock = nullptr;
  ir_node *falseBlock = nullptr;
  
  // to do: there must be a more direct way
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
  
  Label trueLabel = getLabel(trueBlock);
  Label falseLabel = getLabel(falseBlock);
  
  ir_node *l = get_Cmp_left(s);
  ir_node *r = get_Cmp_right(s);
  
  long lreg = registers[get_irn_node_nr(l)];
  long rreg = registers[get_irn_node_nr(r)];
  
  auto cmp = make_shared<cmpl_>();
  cmp->src1 = lreg;
  cmp->src2 = rreg;
  getCurrentBlock()->instructions->push_back(cmp);
  
  auto br = make_shared<Branch>();
  br->relation = get_Cmp_relation(s);
  br->label = trueLabel;
  getCurrentBlock()->instructions->push_back(br);
  
  auto j = make_shared<jmp>();
  j->label = falseLabel;
  getCurrentBlock()->instructions->push_back(j);
  
  getCurrentBlock()->exitInstruction = cmp;
}

void GraphAssembler::buildJmp(ir_node *node) {
  foreach_out_edge_safe(node, edge) {
    ir_node *n = get_edge_src_irn(edge);
    assert(is_Block(n));
    Label l = getLabel(n);
    
    auto j = make_shared<jmp>();
    j->label = l;
    getCurrentBlock()->instructions->push_back(j);
    
    auto lb = getCurrentBlock();
    lb->exitInstruction = j;
    return;
  }
}

void GraphAssembler::buildProj(ir_node *node) {
  ir_node *pred = get_Proj_pred(node);
  if (is_Proj(pred)) {
    ir_node *ppred = get_Proj_pred(pred);
    if (is_Start(ppred)) {
      // Is an argument
      auto m = make_shared<movl_from_stack>();
      m->offset = get_Proj_num(node);
      long oreg = allocateReg(node);
      m->dest = oreg;
      getCurrentBlock()->instructions->push_back(m);
    }
  }
}

void GraphAssembler::buildAdd(ir_node *node) {
  ir_node *l = get_Add_left(node);
  ir_node *r = get_Add_right(node);
  
  long lreg = registers[get_irn_node_nr(l)];
  long rreg = registers[get_irn_node_nr(r)];
  long oreg = allocateReg(node);
  
  auto inst = make_shared<addl>();
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  getCurrentBlock()->instructions->push_back(inst);
}

void GraphAssembler::buildReturn(ir_node *node) {
  if (get_Return_n_ress(node) > 0) {
    ir_node *pred = get_Return_res(node, 0);
    long r = registers[get_irn_node_nr(pred)];
    auto inst = make_shared<movl_to_rax>();
    inst->src1 = r;
    getCurrentBlock()->instructions->push_back(inst);
  }
  
  auto pop = make_shared<popq_rbp>();
  getCurrentBlock()->instructions->push_back(pop);
  auto ret = make_shared<retq>();
  getCurrentBlock()->instructions->push_back(ret);
}


int nodeNum=0;
// for use with irg_walk_topological()
void irgNodeWalker(ir_node *node, void *env)
{
  GraphAssembler *_this = static_cast<GraphAssembler*>(env);
  
  if (is_Block(node)) {
    _this->buildBlock(node);
  }
  
  if (is_Const(node)) {
    _this->buildConst(node);
  }
      
  if (is_Cond(node)) {
    _this->buildCond(node);
  }
  
  if (is_Jmp(node)) {
    _this->buildJmp(node);
  }
  
  if (is_Phi(node)) {
    _this->handlePhi(node);
  }
  
  if (is_Proj(node)) {
    _this->buildProj(node);
  }
    
  if (is_Add(node)) {
    _this->buildAdd(node);
  }
    
  if (is_Return(node)) {
    _this->buildReturn(node);
  }
}





#pragma mark - Methods

string GraphAssembler::run()
{
  // Store number of arguments
  string assemblerOutput;
  blocks = make_shared<map<Label, shared_ptr<LabeledBlock>>>();
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
  labelPrefix = AMD64LabelPrefix + std::to_string(get_irg_graph_nr(irg)) + "_";
  
  // Walk graph
  irg_walk_topological(irg, irgNodeWalker, static_cast<void*>(this));
  
  // Insert prolog
  insertProlog();
}



void GraphAssembler::irgRegisterAllocation()
{
  // work with instructions-vector
  // primitive: just use 2 registers
  for (auto const& label : *labels) {
    auto lb = blocks->at(label);
    auto instructions = lb->instructions;
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
        
        // store dest
        auto m3 = make_shared<movl_to_stack>();
        m3->offset = (unsigned) (i->dest + nargs);
        m3->src = m2->dest;

        i->src1 = m1->dest;
        i->src2 = m2->dest;
        i->dest = i->src2;
        
        instructions_->push_back(m1);
        instructions_->push_back(m2);
        instructions_->push_back(instruction);
        instructions_->push_back(m3);
        continue;
      }
      
      if (auto i = dynamic_cast<I1to1*>(instruction.get())) {
        // load arg1
        auto m1 = make_shared<movl_from_stack>();
        m1->offset = (unsigned) (i->src1 + nargs);
        m1->dest = 0;
        
        // store dest
        auto m3 = make_shared<movl_to_stack>();
        m3->offset = (unsigned) (i->dest + nargs);
        m3->src = 1;
        
        i->src1 = 0;
        i->dest = 1;
        
        instructions_->push_back(m1);
        instructions_->push_back(instruction);
        instructions_->push_back(m3);
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
    
    lb->instructions = instructions_;
  }
  
  // enhanced: from https://en.wikipedia.org/wiki/X86_calling_conventions#x86-64_calling_conventions
  // System V AMD64: 6 times int/pointer {RDI, RSI, RDX, RCX, R8, R9} ["certain" floating point {XMM0–7} probably irrelevant for us] Caller Stack aligned on 16 bytes
  // after the allocation only 6 registers must be used
  // save size of needed stack frame somewhere
}

string GraphAssembler::irgCodeGeneration()
{
  // call generate() for every instruction, set labels, create one long string
  std::string assembler;
  
  for (auto const& label : *labels) {
    assembler += label + ":\n";
    
    auto instructions = blocks->at(label)->instructions;
    
    for (auto const& instruction : *instructions) {
      assembler += "\t" + instruction->generate() + "\n";
    }
  }
  
  return move(assembler);
}
