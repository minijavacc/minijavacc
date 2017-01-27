#include "graphassembler.h"
#include <stdio.h>
#include <vector>
#include <libfirm/firm.h>


using namespace cmpl;

#pragma mark - Misc

inline shared_ptr<LabeledBlock> GraphAssembler::getLabeledBlockForIrNode(ir_node *node)
{
  return blocks.at(nodeNrToLabel.at(get_irn_node_nr(get_nodes_block(node))));
}

shared_ptr<Value> GraphAssembler::getValue(ir_node *node) {
  if (registers.count(get_irn_node_nr(node)) > 0) {
    return registers.at(get_irn_node_nr(node));
  }
  
  auto r = make_shared<Value>(get_irn_mode(node));
  setValue(node, r);
  return r;
}

void GraphAssembler::setValue(ir_node *node, shared_ptr<Value> r) {
  registers.emplace(get_irn_node_nr(node), r);
}

void GraphAssembler::allocValue(shared_ptr<Value> &r) {
  if (r->type == ValueTypeVirtual)
  {
    // decrement global topOfStack to get new offset
    topOfStack -= 8;
    
    r->type = ValueTypeStackSlot;
    r->offset = topOfStack;
  }
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




#pragma mark - Function prolog/epilog

void GraphAssembler::insertProlog() {
  auto p = make_shared<push>(__func__, __LINE__);
  p->src1 = Value::rbp();

  int x = 1; // for iterator
  
  auto m = make_shared<StaticInstruction>("movq %rsp, %rbp", __func__, __LINE__);
  Label fl = labels.front();
  auto bl = blocks.at(fl);
  auto it = bl->instructions.begin();
  bl->instructions.insert(it, p);
  it = bl->instructions.begin();
  bl->instructions.insert(it + x++, m);
  
  if (topOfStack < 0)
  {
    auto s = make_shared<subq_rsp>(__func__, __LINE__);
    s->bytes = (unsigned) topOfStack;
    it = bl->instructions.begin();
    bl->instructions.insert(it + x++, s);
  }

  // get all arguments (that are used) from the registers and save them to the stack
  for (int i = 0; i < 6; i++)
  {
    if (regArgsToValue[i] == nullptr)
    {
      return;
    }
    
    // move parameter to virtual register
    auto inst = make_shared<mov>(__func__, __LINE__);
    inst->dest = regArgsToValue[i];
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
  Label l = nodeNrToLabel.at(get_irn_node_nr(bl));
  shared_ptr<LabeledBlock> lb = blocks.at(l);
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
  blocks.emplace(l, lb);
  labels.push_back(l);
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
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];
  
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
      regArgsToValue[i] = getValue(node);			
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
    if (registers.count(get_irn_node_nr(pred)) > 0)
    {
      setValue(node, registers[get_irn_node_nr(pred)]);
    }
  }
}

void GraphAssembler::buildAdd(ir_node *node) {
  ir_node *l = get_Add_left(node);
  ir_node *r = get_Add_right(node);
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];
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
    auto r = registers[get_irn_node_nr(pred)];
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
    
    assert(registers.count(get_irn_node_nr(a)) > 0);
    shared_ptr<Value> reg = registers[get_irn_node_nr(a)];
    
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
      
      assert(registers.count(get_irn_node_nr(a)) > 0);
      shared_ptr<Value> reg = registers[get_irn_node_nr(a)];
      
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
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];
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
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];
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
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];
  
  auto inst = make_shared<div>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;

  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildMod(ir_node *node) {
  ir_node *l = get_Mod_left(node);
  ir_node *r = get_Mod_right(node);
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];

  auto inst = make_shared<mod>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;

  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildMinus(ir_node *node) {
  ir_node *m = get_Minus_op(node);

  auto mreg = registers[get_irn_node_nr(m)];
  auto oreg = getValue(node);
  
  auto inst = make_shared<neg>(__func__, __LINE__);
  inst->src1 = mreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}





#pragma mark - Physical register allocation

void GraphAssembler::deliverValue(shared_ptr<Value> &src, shared_ptr<Value> &dest, vector<shared_ptr<Instruction>> &instructions)
{
  switch (src->type)
  {
    case ValueTypeVirtual:
      switch (dest->type)
      {
        case ValueTypePhysical:
        {
          src->type = dest->type;
          src->identifier = dest->identifier;
          break;
        }
          
        case ValueTypeStackSlot:
        {
          src->type = dest->type;
          src->offset = dest->offset;
          break;
        }
          
        case ValueTypeVirtual:
        {
          allocValue(dest);
          src->type = dest->type;
          src->offset = dest->offset;
          break;
        }
        
        default:
          assert(false);
      }
      break;
    
    case ValueTypeImmediate:
      switch (dest->type)
      {
        case ValueTypePhysical:
        case ValueTypeStackSlot:
        {
          auto m = make_shared<mov>(__func__, __LINE__);
          m->src1 = src;
          m->dest = dest;
          instructions.push_back(m);
          break;
        }
          
        case ValueTypeVirtual:
        {
          dest->type = src->type;
          dest->immediate = src->immediate;
          break;
        }
        
        default:
          assert(false);
      }
      break;
    
    case ValueTypePhysical:
      switch (dest->type)
      {
        case ValueTypePhysical:
        {
          if (src->identifier == dest->identifier)
            return;
          // no break
        }
        case ValueTypeStackSlot:
        {
          auto m = make_shared<mov>(__func__, __LINE__);
          m->src1 = src;
          m->dest = dest;
          instructions.push_back(m);
          break;
        }
          
        case ValueTypeVirtual:
        {
          dest->type = src->type;
          dest->immediate = src->immediate;
          break;
        }
        
        default:
          assert(false);
      }
      break;
    
    case ValueTypeStackSlot:
      switch (dest->type)
      {
        case ValueTypePhysical:
        {
          auto m = make_shared<mov>(__func__, __LINE__);
          m->src1 = src;
          m->dest = dest;
          instructions.push_back(m);
          break;
        }
        
        case ValueTypeStackSlot:
        {
          if (src->offset == dest->offset)
            return;
          auto m1 = make_shared<mov>(__func__, __LINE__);
          m1->src1 = src;
          m1->dest = Value::r10_(m1->src1->size);
          instructions.push_back(m1);
          auto m2 = make_shared<mov>(__func__, __LINE__);
          m2->src1 = Value::r10_(m1->src1->size);
          m2->dest = dest;
          instructions.push_back(m2);
          break;
        }
          
        case ValueTypeVirtual:
        {
          dest->type = src->type;
          dest->immediate = src->immediate;
          break;
        }
        
        default:
          assert(false);
      }
      break;
    
    default:
      assert(false);
  }
}


void GraphAssembler::allocI2to1(shared_ptr<Instruction> instr, I2to1 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  // the first operand (src1 and dest) must not be a memory location
  // because we never map two different values on the same stack frame
  
  auto r = Value::r10_(i->src1->size);
  
  deliverValue(i->src1, r, instructions_);
  instructions_.push_back(instr);
  deliverValue(r, i->dest, instructions_);
  
  allocValue(i->src2);
  
  i->src1 = r;
  i->dest = r;
}

void GraphAssembler::allocI2to0(shared_ptr<Instruction> instr, I2to0 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  allocValue(i->src1);
  allocValue(i->src2);
  
  // ony one operand can be a memory access
  if (i->src1->type == ValueTypeStackSlot && 
      i->src2->type == ValueTypeStackSlot)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src1, r1, instructions_);
    i->src1 = r1;
  }
  
  // src2 must not be an immediate
  if (i->src2->type == ValueTypeImmediate)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src2, r1, instructions_);
    i->src2 = r1;
  }
  
  // src2 as memory and src1 as immediate is not allowed
  if (i->src2->type == ValueTypeStackSlot && 
      i->src1->type == ValueTypeImmediate)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src2, r1, instructions_);
    i->src2 = r1;
  }
  
  instructions_.push_back(instr);
}

void GraphAssembler::allocI1to1(shared_ptr<Instruction> instr, I1to1 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  allocValue(i->src1);
  allocValue(i->dest);
  
  // ony one operand can be a memory access
  if (i->src1->type == ValueTypeStackSlot && 
      i->dest->type == ValueTypeStackSlot)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src1, r1, instructions_);
    i->src1 = r1;
  }
  
  instructions_.push_back(instr);
}

void GraphAssembler::allocI1to0(shared_ptr<Instruction> instr, I1to0 *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  allocValue(i->src1);
  
  instructions_.push_back(instr);
}

void GraphAssembler::allocI0to1(shared_ptr<Instruction> instr, I0to1 *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  allocValue(i->dest);
  
  instructions_.push_back(instr);
}

void GraphAssembler::allocDiv(shared_ptr<Instruction> instr, div *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
	//result in:
  Register::eax();
  instructions_.push_back(instr);
}

void GraphAssembler::allocMod(shared_ptr<Instruction> instr, mod *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
	//result in:
  Register::edx();
  instructions_.push_back(instr);
}

void GraphAssembler::allocCall(shared_ptr<Instruction> instr, call *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  // if function returns void, i->dest is NULL
  
  instructions_.push_back(instr);
  
  if (i->dest != nullptr)
  {
    // move result to stack
    auto ax = Value::_ax(i->dest->size);
    deliverValue(ax, i->dest, instructions_);
  }
}

void GraphAssembler::allocMove(shared_ptr<Instruction> instr, mov *i, vector<shared_ptr<Instruction>> &instructions_)
{
  allocValue(i->src1);
  allocValue(i->dest);
  
  if (i->src1->type == ValueTypeStackSlot && 
      i->dest->type == ValueTypeStackSlot)
  {
    auto r1 = Value::r10_(i->src1->size);
    deliverValue(i->src1, r1, instructions_);
    i->src1 = r1;
  }
  
  instructions_.push_back(instr);
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
  blocks = map<Label, shared_ptr<LabeledBlock>>();
  labels = vector<Label>();
  ir_entity *ent = get_irg_entity(irg);
  ir_type *ty = get_entity_type(ent);
  nargs = get_method_n_params(ty);

  
  lower_highlevel_graph(irg);
  
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
  
  // Handle phis
  phiInsertion();
  
  // Finalise (append exit instructions to instruction list)
  for (auto pair : blocks) {
    auto bl = pair.second;
    bl->finalize();
  }
  
  // Insert prolog
  insertProlog();
}



void GraphAssembler::phiInsertion()
{
  for (auto const& l : labels) {
    auto lb = blocks.at(l);
    
    // Step 2: Load all phis into temporary helper registers
    for (auto const& phi : lb->phis) {
      auto bl = get_nodes_block(phi);
      
      for (int i = 0; i < get_Phi_n_preds(phi); i++) {
        ir_node *phipred = get_Phi_pred(phi, i);
        auto inReg = getValue(phipred);
        auto helper = getValue(phi);
        
        ir_node *j = get_Block_cfgpred(bl, i);
        ir_node *jbl = get_nodes_block(j);
        
        Label l = nodeNrToLabel.at(get_irn_node_nr(jbl));
        shared_ptr<LabeledBlock> lb = blocks.at(l);
        
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



void GraphAssembler::irgRegisterAllocation()
{
  // work with instructions-vector
  // primitive: just use 2 registers
  for (auto const& label : labels) {
    auto lb = blocks.at(label);
    auto instructions = lb->instructions;
    auto instructions_ = vector<shared_ptr<Instruction>>();
    
    for (auto const& instruction : instructions) {
      if (auto i = dynamic_cast<I2to1*>(instruction.get())) {
        allocI2to1(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<div*>(instruction.get())) {
		allocDiv(instruction, i, instructions_);
		
      } else if (auto i = dynamic_cast<mod*>(instruction.get())) {
		allocMod(instruction, i, instructions_);
		
      } else if (auto i = dynamic_cast<I2to0*>(instruction.get())) {
		allocI2to0(instruction, i, instructions_);
		
      } else if (auto i = dynamic_cast<I1to1*>(instruction.get())) {
        allocI1to1(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<I1to0*>(instruction.get())) {
        allocI1to0(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<I0to1*>(instruction.get())) {
        allocI0to1(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<call*>(instruction.get())) {
        allocCall(instruction, i, instructions_);
        
      } else if (auto i = dynamic_cast<mov*>(instruction.get())) {
        allocMove(instruction, i, instructions_);
        
      } else {
        instructions_.push_back(instruction);
        
      }
    }
    
    lb->instructions = std::move(instructions_);
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
  
  auto ldname = std::string(get_entity_ld_name(get_irg_entity(irg)));
  
  assembler += ".p2align 4,,15\n";
  assembler += ".type " + ldname + ", @function\n";
  
  for (auto const& label : labels) {
    assembler += label + ":\n";
    
    auto instructions = blocks.at(label)->instructions;
    
    for (auto const& instruction : instructions) {
      assembler += "\t" + instruction->generate() + "\n";
    }
  }
  
  assembler += ".size " + ldname + ", .-" + ldname + "\n\n";
  
  return move(assembler);
}
