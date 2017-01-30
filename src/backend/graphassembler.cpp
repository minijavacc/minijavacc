#include "graphassembler.h"
#include <stdio.h>
#include <vector>
#include <libfirm/firm.h>


using namespace cmpl;

#pragma mark - Misc

inline shared_ptr<LabeledBlock> GraphAssembler::getLabeledBlockForIrNode(ir_node *node)
{
  return context->blocks.at(context->nodeNrToLabel.at(get_irn_node_nr(get_nodes_block(node))));
}

shared_ptr<Value> GraphAssembler::getValue(ir_node *node) {
  if (context->registers.count(get_irn_node_nr(node)) > 0) {
    return context->registers.at(get_irn_node_nr(node));
  }
  
  auto r = make_shared<Value>(get_irn_mode(node));
  setValue(node, r);
  return r;
}

void GraphAssembler::setValue(ir_node *node, shared_ptr<Value> r) {
  context->registers.emplace(get_irn_node_nr(node), r);
}

Label GraphAssembler::getLabel(ir_node *node) {
  if (context->nodeNrToLabel.count(get_irn_node_nr(node)) > 0) {
    return context->nodeNrToLabel.at(get_irn_node_nr(node));
  }
  
  // if is the first label, take the function name
  if (context->nodeNrToLabel.empty())
  {
    Label l = get_entity_ld_name(get_irg_entity(irg));
    context->nodeNrToLabel.emplace(get_irn_node_nr(node), l);
    return l;
  }
  else
  {
    string p = context->labelPrefix;
    Label l = p + to_string(context->nextFreeLabel);
    context->nodeNrToLabel.emplace(get_irn_node_nr(node), l);
    context->nextFreeLabel = context->nextFreeLabel + 1;
    return l;
  }
}




#pragma mark - Function prolog/epilog

void GraphAssembler::insertProlog() {
  auto p = make_shared<push>(__func__, __LINE__);
  p->src1 = Value::rbp();

  int x = 1; // for iterator
  
  auto m = make_shared<StaticInstruction>("movq %rsp, %rbp", __func__, __LINE__);
  Label fl = context->labels.front();
  auto bl = context->blocks.at(fl);
  auto it = bl->instructions.begin();
  bl->instructions.insert(it, p);
  it = bl->instructions.begin();
  bl->instructions.insert(it + x++, m);
  
  if (context->topOfStack < 0)
  {
    auto s = make_shared<subq_rsp>(__func__, __LINE__);
    s->bytes = (unsigned) context->topOfStack;
    it = bl->instructions.begin();
    bl->instructions.insert(it + x++, s);
  }

  // get all arguments (that are used) from the registers and save them to the stack
  for (int i = 0; i < 6; i++)
  {
    if (context->regArgsToValue[i] == nullptr)
    {
      return;
    }
    
    // move parameter to virtual register
    auto inst = make_shared<mov>(__func__, __LINE__);
    inst->dest = context->regArgsToValue[i];
    auto size = inst->dest->size;
    
    switch (i)
    {
      case 0:
        inst->src1 = Value::_di(size);
        break;
      
      case 1:
        inst->src1 = Value::_si(size);
        break;
        
      case 2:
        inst->src1 = Value::_dx(size);
        break;
      
      case 3:
        inst->src1 = Value::_cx(size);
        break;
      
      case 4:
        inst->src1 = Value::r8_(size);
        break;
      
      case 5:
        inst->src1 = Value::r9_(size);
        break;
      
      default:
        assert(false);
    }
    
    it = bl->instructions.begin();
    bl->instructions.insert(it + x++, inst);
  }
}

// Epilog is implicit in buildReturn()
// move to single location to avoid code duplication?






#pragma mark - Instruction builders

void GraphAssembler::collectPhi(ir_node *node) {
  // Step 1: Collect all phi nodes of every block
  ir_node *bl = get_nodes_block(node);
  Label l = context->nodeNrToLabel.at(get_irn_node_nr(bl));
  shared_ptr<LabeledBlock> lb = context->blocks.at(l);
  lb->phis.push_back(node);
  
  // Important: destination register has to be known right after this point in time
  // We allocate it now
  getValue(node);
}

void GraphAssembler::buildBlock(ir_node *node) {
  ir_graph *g = get_irn_irg(node);
  if (node == get_irg_end_block(g)) {
    return;
  }
  
  Label l = getLabel(node);
  auto lb = make_shared<LabeledBlock>();
  lb->label = l;
  context->blocks.emplace(l, lb);
  context->labels.push_back(l);
}

void GraphAssembler::buildConst(ir_node *node) {
  ir_tarval *val = get_Const_tarval(node);
  long l = get_tarval_long(val); // TODO: can value exceed type integer? (see -(int) )
  
  auto r = make_shared<Value>(get_irn_mode(node), l);
  setValue(node, r);
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
  
  auto lreg = context->registers[get_irn_node_nr(l)];
  auto rreg = context->registers[get_irn_node_nr(r)];
  
  auto comp = make_shared<cmp>(__func__, __LINE__);
  comp->src1 = lreg;
  comp->src2 = rreg;
//  getCurrentBlock()->instructions.push_back(cmp);
  getLabeledBlockForIrNode(node)->exitInstructions.push_back(comp);
  
  auto br = make_shared<Branch>(__func__, __LINE__);
  br->relation = get_Cmp_relation(s);
  br->label = trueLabel;
//  getCurrentBlock()->instructions.push_back(br);
  getLabeledBlockForIrNode(node)->exitInstructions.push_back(br);
  
  auto j = make_shared<jmp>(__func__, __LINE__);
  j->label = falseLabel;
//  getCurrentBlock()->instructions.push_back(j);
  getLabeledBlockForIrNode(node)->exitInstructions.push_back(j);
}

void GraphAssembler::buildJmp(ir_node *node) {
  foreach_out_edge_safe(node, edge) {
    ir_node *n = get_edge_src_irn(edge);
    assert(is_Block(n));
    Label l = getLabel(n);
    
    auto j = make_shared<jmp>(__func__, __LINE__);
    j->label = l;
//    getCurrentBlock()->instructions.push_back(j);
    
    getLabeledBlockForIrNode(node)->exitInstructions.push_back(j);
    return;
  }
}

void GraphAssembler::buildProj(ir_node *node) {
  ir_node *pred = get_Proj_pred(node);
  ir_node *ppred;
  
  // check if Proj gets an argument
  if (is_Proj(pred) && (ppred = get_Proj_pred(pred)) && is_Start(ppred)) {
    
    // get arg index
    unsigned int i = get_Proj_num(node);
    auto size = Value::valueSizeFromIRMode(get_irn_mode(node));
    
    // if one of the first 6 arguments, add shared_ptr to regArgsToValue 
    // and generate mov instructions in insertProlog
    if (i < 6)
    {
      context->regArgsToValue[i] = getValue(node);			
    }
    // if argument > 5
    else
    {
      auto r = make_shared<Value>(size, i * 8);
      setValue(node, r);
    }
  }
  else
  {
    // else: if predecessor maps to a register, map to the same one
    // hacky, but should work
    if (context->registers.count(get_irn_node_nr(pred)) > 0)
    {
      setValue(node, context->registers[get_irn_node_nr(pred)]);
    }
  }
}

void GraphAssembler::buildAdd(ir_node *node) {
  ir_node *l = get_Add_left(node);
  ir_node *r = get_Add_right(node);
  
  auto lreg = context->registers[get_irn_node_nr(l)];
  auto rreg = context->registers[get_irn_node_nr(r)];
  auto oreg = getValue(node);
  
  auto inst = make_shared<add>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;

  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildReturn(ir_node *node) {
  if (get_Return_n_ress(node) > 0) {
    ir_node *pred = get_Return_res(node, 0);
    auto r = context->registers[get_irn_node_nr(pred)];
    auto inst = make_shared<mov>(__func__, __LINE__);
    inst->src1 = r;
    inst->dest = Value::_ax(inst->src1->size);
    getLabeledBlockForIrNode(node)->instructions.push_back(inst);
  }
  
  auto po = make_shared<pop>(__func__, __LINE__);
  po->dest = Value::rbp();
  getLabeledBlockForIrNode(node)->instructions.push_back(po);
  auto reti = make_shared<ret>(__func__, __LINE__);
  getLabeledBlockForIrNode(node)->instructions.push_back(reti);
}

void GraphAssembler::buildCall(ir_node *node) {
  ir_entity *e = get_Call_callee(node);
  ir_type* t = get_entity_type(e);
  
  // use standardized x86_64 calling convention: 
  // http://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64
  // "According to the ABI, the first 6 integer or pointer arguments to a function are passed in registers."
  
  int paramsNum = get_Call_n_params(node);
  
  // iterate over first 6 parameters for the registers
  for (int i = 0; i < 6 && i < paramsNum; i++)
  {
    ir_node *a = get_Call_param(node, i);
    
    assert(context->registers.count(get_irn_node_nr(a)) > 0);
    shared_ptr<Value> reg = context->registers[get_irn_node_nr(a)];
    
    // move parameter to physical register
    auto inst = make_shared<mov>(__func__, __LINE__);
    inst->src1 = reg;
    
    switch (i)
    {
      case 0:
        inst->dest = Value::_di(inst->src1->size);
        break;
      
      case 1:
        inst->dest = Value::_si(inst->src1->size);
        break;
        
      case 2:
        inst->dest = Value::_dx(inst->src1->size);
        break;
      
      case 3:
        inst->dest = Value::_cx(inst->src1->size);
        break;
      
      case 4:
        inst->dest = Value::r8_(inst->src1->size);
        break;
      
      case 5:
        inst->dest = Value::r9_(inst->src1->size);
        break;
      
      default:
        assert(false);
    }
    
    getLabeledBlockForIrNode(node)->instructions.push_back(inst);
  }
  
  // do exist more than 6 parameters?
  if (paramsNum > 5)
  {
    // change stack pointer
    auto s = make_shared<subq_rsp>(__func__, __LINE__);
    s->bytes = (unsigned) 8 * (paramsNum - 6);
    getLabeledBlockForIrNode(node)->instructions.push_back(s);

    // iterate over the remaining parameters for the stack (reversed order)
    for (int i = paramsNum - 1; i > 5 && i < paramsNum; i--)
    {
      ir_node *a = get_Call_param(node, i);
      
      assert(context->registers.count(get_irn_node_nr(a)) > 0);
      shared_ptr<Value> reg = context->registers[get_irn_node_nr(a)];
      
      // create mov instruction to move value to stack
      auto m = make_shared<mov>(reg, (i - 6) * 8, __func__, __LINE__);
      getLabeledBlockForIrNode(node)->instructions.push_back(m);
    }
  }
  
  // save old stack pointer
  getLabeledBlockForIrNode(node)->instructions.push_back(make_shared<StaticInstruction>("pushq %rsp", __func__, __LINE__));
  getLabeledBlockForIrNode(node)->instructions.push_back(make_shared<StaticInstruction>("pushq (%rsp)", __func__, __LINE__));
  
  // align base pointer to 2^8
  getLabeledBlockForIrNode(node)->instructions.push_back(make_shared<StaticInstruction>("andq $-0x10, %rsp", __func__, __LINE__));
  
  // call the function
  const char* ldname = get_entity_ld_name(e);
  auto c = make_shared<call>(__func__, __LINE__);
  c->label = ldname;
  
  // do exist more than 6 parameters?
  if (paramsNum > 5)
  {
    // change stack pointer
    auto s = make_shared<addq_rsp>(__func__, __LINE__);
    s->bytes = (unsigned) 8 * (paramsNum - 6);
    getLabeledBlockForIrNode(node)->instructions.push_back(s);
  }
  
  // if non-void function: allocate virtual register
  if (get_method_n_ress(t) > 0)
  {
    c->dest = getValue(node);
  }
  
  getLabeledBlockForIrNode(node)->instructions.push_back(c);
  
  // restore old stack pointer
  getLabeledBlockForIrNode(node)->instructions.push_back(make_shared<StaticInstruction>("movq 8(%rsp), %rsp", __func__, __LINE__));
  
  // if non-void function: move return value to virtual register
  if (get_method_n_ress(t) > 0)
  {
    ir_mode* m = get_type_mode(t);
    setValue(node, c->dest);
  }
}

void GraphAssembler::buildSub(ir_node *node) {
  ir_node *l = get_Sub_left(node);
  ir_node *r = get_Sub_right(node);
  
  auto lreg = context->registers[get_irn_node_nr(l)];
  auto rreg = context->registers[get_irn_node_nr(r)];
  auto oreg = getValue(node);
  
  auto inst = make_shared<sub>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildMul(ir_node *node) {
  ir_node *l = get_Mul_left(node);
  ir_node *r = get_Mul_right(node);
  
  auto lreg = context->registers[get_irn_node_nr(l)];
  auto rreg = context->registers[get_irn_node_nr(r)];
  auto oreg = getValue(node);
  
  auto inst = make_shared<imul>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildDiv(ir_node *node) {
  ir_node *l = get_Div_left(node);
  ir_node *r = get_Div_right(node);
  
  auto lreg = context->registers[get_irn_node_nr(l)];
  auto rreg = context->registers[get_irn_node_nr(r)];
 
  auto inst = make_shared<div>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->result=getValue(node);
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildMod(ir_node *node) {
  ir_node *l = get_Mod_left(node);
  ir_node *r = get_Mod_right(node);
  
  auto lreg = context->registers[get_irn_node_nr(l)];
  auto rreg = context->registers[get_irn_node_nr(r)];

  auto inst = make_shared<mod>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->result=getValue(node);
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildMinus(ir_node *node) {
  ir_node *m = get_Minus_op(node);

  auto mreg = context->registers[get_irn_node_nr(m)];
  auto oreg = getValue(node);
  
  auto inst = make_shared<neg>(__func__, __LINE__);
  inst->src1 = mreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}



int nodeNum=0;
// for use with irg_walk_topological()
void irgNodeWalker(ir_node *node, void *env)
{
  GraphAssembler *_this = static_cast<GraphAssembler*>(env);
  
  if (is_Block(node)) {
    _this->buildBlock(node);
  }  
  else if (is_Const(node)) {
    _this->buildConst(node);
  }
  else if (is_Cond(node)) {
    _this->buildCond(node);
  }
  else if (is_Jmp(node)) {
    _this->buildJmp(node);
  }
  else if (is_Phi(node)) {
    _this->collectPhi(node);
  }
  else if (is_Proj(node)) {
    _this->buildProj(node);
  }    
  else if (is_Add(node)) {
    _this->buildAdd(node);
  }    
  else if (is_Return(node)) {
    _this->buildReturn(node);
  }
  else if (is_Call(node)) {
    _this->buildCall(node);
  }
  else if (is_Sub(node)) {
    _this->buildSub(node);
  }
  else if (is_Mul(node)) {
    _this->buildMul(node);
  }
  else if (is_Div(node)) {
    _this->buildDiv(node);
  }
  else if (is_Mod(node)) {
    _this->buildMod(node);
  }  
  else if (is_Minus(node)) {
    _this->buildMinus(node);
  }
  else {
    // not every node type needs a buildNode function!
  }
  
}





#pragma mark - Methods

string GraphAssembler::run()
{  
  // Store number of arguments

  string assemblerOutput;
  context->blocks = map<Label, shared_ptr<LabeledBlock>>();
  context->labels = vector<Label>();
  ir_entity *ent = get_irg_entity(irg);
  ir_type *ty = get_entity_type(ent);
  context->nargs = get_method_n_params(ty);

  
  lower_highlevel_graph(irg);
  
  // activate all edges in graph
  edges_activate(irg);
  
  irgSerialize();
  
  
  registerAllocator->registerAllocation(context);
  assemblerOutput = irgCodeGeneration();
  
  // deactivate edges
  edges_deactivate(irg);
  
  return move(assemblerOutput);
}



void GraphAssembler::irgSerialize()
{
  context->labelPrefix = AMD64LabelPrefix + std::to_string(get_irg_graph_nr(irg)) + "_";
  
  // Walk graph
  irg_walk_topological(irg, irgNodeWalker, static_cast<void*>(this));
  
  // Handle phis
  phiInsertion();
  
  // Finalise (append exit instructions to instruction list)
  for (auto pair : context->blocks) {
    auto bl = pair.second;
    bl->finalize();
  }
  
  // Insert prolog
  insertProlog();
}



void GraphAssembler::phiInsertion()
{
  for (auto const& l : context->labels) {
    auto lb = context->blocks.at(l);
    
    // Step 2: Load all phis into temporary helper registers
    for (auto const& phi : lb->phis) {
      auto bl = get_nodes_block(phi);
      
      for (int i = 0; i < get_Phi_n_preds(phi); i++) {
        ir_node *phipred = get_Phi_pred(phi, i);
        auto inReg = getValue(phipred);
        auto helper = getValue(phi);
        
        ir_node *j = get_Block_cfgpred(bl, i);
        ir_node *jbl = get_nodes_block(j);
        
        Label l = context->nodeNrToLabel.at(get_irn_node_nr(jbl));
        shared_ptr<LabeledBlock> lb = context->blocks.at(l);
        
        auto m = make_shared<mov>(__func__, __LINE__); TODO:
        m->src1 = inReg;
        m->dest = helper;
        
        lb->instructions.push_back(m);
      }
    }
    
    /* TODO: can this work without step 3?
    // Step 3: Load all phis from temporary helper to out register
    for (auto const& phi : lb->phis) {
      auto bl = get_nodes_block(phi);
      auto outReg = getValue(phi);
      
      for (int i = 0; i < get_Phi_n_preds(phi); i++) {
        ir_node *phipred = get_Phi_pred(phi, i);
        auto helper = getValue(phipred);
        
        ir_node *j = get_Block_cfgpred(bl, i);
        ir_node *jbl = get_nodes_block(j);
        
        Label l = nodeNrToLabel.at(get_irn_node_nr(jbl));
        shared_ptr<LabeledBlock> lb = blocks.at(l);
        
        auto m = make_shared<mov_old>(__func__, __LINE__);
        m->src1 = helper;
        m->dest = outReg;
        
        lb->instructions.push_back(m);
      }
    }
    */
  }
}

string GraphAssembler::irgCodeGeneration()
{
  // call generate() for every instruction, set labels, create one long string
  std::string assembler;
  
  auto ldname = std::string(get_entity_ld_name(get_irg_entity(irg)));
  
  assembler += ".p2align 4,,15\n";
  assembler += ".type " + ldname + ", @function\n";
  
  for (auto const& label : context->labels) {
    assembler += label + ":\n";
    
    auto instructions = context->blocks.at(label)->instructions;
    
    for (auto const& instruction : instructions) {
      assembler += "\t" + instruction->generate() + "\n";
    }
  }
  
  assembler += ".size " + ldname + ", .-" + ldname + "\n\n";
  
  return move(assembler);
}
