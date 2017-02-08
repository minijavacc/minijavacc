#include "graphassembler.h"
#include <stdio.h>
#include <vector>
#include <libfirm/firm.h>
#include "values.h"


using namespace cmpl;

#pragma mark - Misc

inline shared_ptr<LabeledBlock> GraphAssembler::getLabeledBlockForIrNode(ir_node *node)
{
  auto l = getLabel(get_nodes_block(node));
  
  if (blocks->count(l) > 0) {
    return blocks->at(l);
  }
  
  auto lb = make_shared<LabeledBlock>();
  lb->label = l;
  blocks->emplace(l, lb);
  
  return lb;
}

shared_ptr<Value> GraphAssembler::getValue(ir_node *node) {
  if (values.count(get_irn_node_nr(node)) > 0) {
    return values.at(get_irn_node_nr(node));
  }
  
  auto r = make_shared<Virtual>(node);
  setValue(node, r);
  return r;
}

void GraphAssembler::setValue(ir_node *node, shared_ptr<Value> r) {
  values.emplace(get_irn_node_nr(node), r);
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




#pragma mark - Function prolog

void GraphAssembler::insertParameterMovs() {
  Label fl = labels->front();
  auto bl = blocks->at(fl);
  auto it = bl->instructions.begin();
  int x = 0; // for iterator
  
  // get all arguments (that are used) from the registers and save them to the stack
  for (int i = 0; i < 6; i++)
  {
    if (regArgsToValue[i] == nullptr)
    {
      continue;
    }
    
    // move parameter to stack slot
    auto inst = make_shared<mov>(__func__, __LINE__);
    inst->dest = regArgsToValue[i]->getLowered(stackFrameAllocation);
    auto size = inst->dest->getSize();
    
    switch (i)
    {
      case 0:
        inst->src1 = make_shared<Physical>(ID_DI, size);
        break;
        
      case 1:
        inst->src1 = make_shared<Physical>(ID_SI, size);;
        break;
        
      case 2:
        inst->src1 = make_shared<Physical>(ID_DX, size);;
        break;
        
      case 3:
        inst->src1 = make_shared<Physical>(ID_CX, size);;
        break;
        
      case 4:
        inst->src1 = make_shared<Physical>(ID_08, size);;
        break;
        
      case 5:
        inst->src1 = make_shared<Physical>(ID_09, size);;
        break;
        
      default:
        assert(false);
    }
    
    it = bl->instructions.begin();
    bl->instructions.insert(it + x++, inst);
  }
}

void GraphAssembler::insertProlog() {
  Label fl = labels->front();
  auto bl = blocks->at(fl);
  
  auto p = make_shared<push>(__func__, __LINE__);
  p->src1 = make_shared<Physical>(ID_BP, ValueSize64);

  
  auto m = make_shared<StaticInstruction>("movq %rsp, %rbp", __func__, __LINE__);
  auto it = bl->instructions.begin();
  bl->instructions.insert(it, p);
  it = bl->instructions.begin();
  bl->instructions.insert(it + 1, m);
  
  if (stackFrameAllocation->getTopOfStack() < 0)
  {
    auto s = make_shared<subq_rsp>(__func__, __LINE__);
    s->bytes = -stackFrameAllocation->getTopOfStack();
    it = bl->instructions.begin();
    bl->instructions.insert(it + 2, s);
  }
}






#pragma mark - Instruction builders

void GraphAssembler::collectPhi(ir_node *node) {
  // ignore mem phi
  if (get_irn_mode(node) == mode_M) {
    return;
  }
  
  // Step 1: Collect all phi nodes of every block
  auto lb = getLabeledBlockForIrNode(node);
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
  
  // getLabeledBlockForIrNode() cannot be used because node is the parent block already
  Label l = getLabel(node);
  auto lb = make_shared<LabeledBlock>();
  lb->label = l;
  blocks->emplace(l, lb);
  
  labels->push_back(l);
}

void GraphAssembler::buildConst(ir_node *node) {
  ir_tarval *val = get_Const_tarval(node);
  long l = get_tarval_long(val); // TODO: can value exceed type integer? (see -(int) )
  
  auto r = make_shared<Immediate>(l, get_irn_mode(node));
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
  
  auto lreg = getValue(l);
  auto rreg = getValue(r);
  
  auto comp = make_shared<cmp>(__func__, __LINE__);
  comp->src1 = rreg; // weird, but correct
  comp->src2 = lreg; // weird, but correct
  getLabeledBlockForIrNode(node)->exitInstructions.push_back(comp);
  
  auto br = make_shared<Branch>(__func__, __LINE__);
  br->relation = get_Cmp_relation(s);
  br->label = trueLabel;
  getLabeledBlockForIrNode(node)->exitInstructions.push_back(br);
  
  auto j = make_shared<jmp>(__func__, __LINE__);
  j->label = falseLabel;
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
  
  // ignore mem proj
  if (get_irn_mode(node) == mode_M) {
    return;
  }
  
  // check if Proj gets an argument
  if (is_Proj(pred) && (ppred = get_Proj_pred(pred)) && is_Start(ppred)) {
    
    // get arg index
    unsigned int i = get_Proj_num(node);
    auto size = Value::valueSizeFromIRNode(node);
    
    // if one of the first 6 arguments, add shared_ptr to regArgsToValue 
    // and generate mov instructions in insertProlog
    if (i < 6)
    {
      if (!regArgsToValue[i]) {
        regArgsToValue[i] = getValue(node);
      } else {
        setValue(node, regArgsToValue[i]);
      }
    }
    // if argument > 5
    else
    {
      auto rbp = make_shared<Physical>(ID_BP, ValueSize64);
      auto r = make_shared<Memory>(rbp, i * 8, size);
      setValue(node, r);
    }
  }
  else
  {
    // else: if predecessor maps to a register, map to the same one
    // hacky, but should work
    if (values.count(get_irn_node_nr(pred)) > 0)
    {
      auto v = getValue(pred);
      setValue(node, v);
    }
  }
}

void GraphAssembler::buildAdd(ir_node *node) {
  ir_node *l = get_Add_left(node);
  ir_node *r = get_Add_right(node);
  
  auto lreg = getValue(l);
  auto rreg = getValue(r);
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
    auto r = getValue(pred);
    auto inst = make_shared<mov>(__func__, __LINE__);
    inst->src1 = r;
    inst->dest = make_shared<Physical>(ID_AX, inst->src1->getSize());
    getLabeledBlockForIrNode(node)->instructions.push_back(inst);
  }
  
  auto rbp = make_shared<Physical>(ID_BP, ValueSize64);
  auto rsp = make_shared<Physical>(ID_SP, ValueSize64);
  
  auto mv = make_shared<mov>(__func__, __LINE__);
  mv->src1 = rbp;
  mv->dest = rsp;
  getLabeledBlockForIrNode(node)->instructions.push_back(mv);
  
  auto po = make_shared<pop>(__func__, __LINE__);
  po->dest = rbp;
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
    
    assert(values.count(get_irn_node_nr(a)) > 0);
    shared_ptr<Value> reg = getValue(a);
    
    // move parameter to physical register
    auto inst = make_shared<mov>(__func__, __LINE__);
    inst->src1 = reg;
    
    switch (i)
    {
      case 0:
        inst->dest = make_shared<Physical>(ID_DI, inst->src1->getSize());
        break;
      
      case 1:
        inst->dest = make_shared<Physical>(ID_SI, inst->src1->getSize());
        break;
        
      case 2:
        inst->dest = make_shared<Physical>(ID_DX, inst->src1->getSize());
        break;
      
      case 3:
        inst->dest = make_shared<Physical>(ID_CX, inst->src1->getSize());
        break;
      
      case 4:
        inst->dest = make_shared<Physical>(ID_08, inst->src1->getSize());
        break;
      
      case 5:
        inst->dest = make_shared<Physical>(ID_09, inst->src1->getSize());
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
      
      assert(values.count(get_irn_node_nr(a)) > 0);
      shared_ptr<Value> reg = getValue(a);
      
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
//  if (get_method_n_ress(t) > 0)
//  {
//    setValue(node, c->dest);
//  }
}

void GraphAssembler::buildSub(ir_node *node) {
  ir_node *l = get_Sub_left(node);
  ir_node *r = get_Sub_right(node);
  
  auto lreg = getValue(l);
  auto rreg = getValue(r);
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
  
  auto lreg = getValue(l);
  auto rreg = getValue(r);
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
  
  auto lreg = getValue(l);
  auto rreg = getValue(r);
  auto oreg = getValue(node);
  
  // Special case: ir_mode of mod is Tuple which results in wrong sized value from getValue()
  // Fix by settings size manually
  if (mode_is_int(get_Div_resmode(node))) {
    oreg->setSize(ValueSize32);
  } else {
    oreg->setSize(ValueSize64);
  }
  
  auto inst = make_shared<div>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildMod(ir_node *node) {
  ir_node *l = get_Mod_left(node);
  ir_node *r = get_Mod_right(node);
  
  auto lreg = getValue(l);
  auto rreg = getValue(r);
  auto oreg = getValue(node);
  
  // Special case: ir_mode of mod is Tuple which results in wrong sized value from getValue()
  // Fix by settings size manually
  if (mode_is_int(get_Mod_resmode(node))) {
    oreg->setSize(ValueSize32);
  } else {
    oreg->setSize(ValueSize64);
  }

  auto inst = make_shared<mod>(__func__, __LINE__);
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildMinus(ir_node *node) {
  ir_node *m = get_Minus_op(node);

  auto mreg = getValue(m);
  auto oreg = getValue(node);
  
  auto inst = make_shared<neg>(__func__, __LINE__);
  inst->src1 = mreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildLoad(ir_node *node) {
  ir_node *memloc = get_Load_ptr(node);
  
  auto src = getValue(memloc);
  auto src_lowered = src->getLowered(stackFrameAllocation);
  auto dest = getValue(node);
  
  auto r = make_shared<Physical>(ID_09, src_lowered->getSize()); // Use r09, since r10 is used for register allocator
  auto src_mov = src_lowered->movToPhysical(r);
  
  auto src_ = make_shared<Memory>(r, 0, dest->getSize());
  
  auto inst = make_shared<mov>(__func__, __LINE__);
  inst->src1 = src_;
  inst->dest = dest;
  
  getLabeledBlockForIrNode(node)->instructions.push_back(src_mov);
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildStore(ir_node *node) {
  ir_node *memloc = get_Store_ptr(node);
  ir_node *value = get_Store_value(node);
  
  auto src = getValue(value);
  auto size = src->getSize();
  auto dest = getValue(memloc);
  auto dest_lowered = dest->getLowered(stackFrameAllocation);
  
  // Set size explicitely
  // dest is a pointer (64 bit) but could be pointing to an int (32 bit)
//  dest_lowered->setSize(size);
  
  auto r = make_shared<Physical>(ID_09, ValueSize64); // Use r09, since r10 is used for register allocator
  auto dest_mov = dest_lowered->movToPhysical(r);
  
  auto dest_ = make_shared<Memory>(r, 0, size);
  
  auto inst = make_shared<mov>(__func__, __LINE__);
  inst->src1 = src;
  inst->dest = dest_;
  
  getLabeledBlockForIrNode(node)->instructions.push_back(dest_mov);
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildConv(ir_node *node) {
  ir_node *op = get_Conv_op(node);
  
  auto inst = make_shared<conv>(__func__, __LINE__);
  inst->src1 = getValue(op);
  inst->dest = getValue(node);
  
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
  else if (is_Load(node)) {
    _this->buildLoad(node);
  }
  else if (is_Store(node)) {
    _this->buildStore(node);
  }
  else if (is_Conv(node)) {
    _this->buildConv(node);
  }
  else {
    // not every node type needs a buildNode function!
  }
  
}





#pragma mark - Methods

string GraphAssembler::run()
{
  // Init main data structures
  blocks = make_shared<map<Label, shared_ptr<LabeledBlock>>>();
  labels = make_shared<vector<Label>>();
  
  // Store number of arguments
  string assemblerOutput;
  ir_entity *ent = get_irg_entity(irg);
  ir_type *ty = get_entity_type(ent);
  nargs = get_method_n_params(ty);

  // lower sels
  lower_highlevel_graph(irg);
  
  // activate all edges in graph
  edges_activate(irg);
  
  // collect nodes to list of virutal instructions list
  irgSerialize();
  
  // Register allocation
  registerAllocator = new RegisterAllocator(blocks, labels, stackFrameAllocation);
  registerAllocator->run();
  delete registerAllocator;
  
  // Insert parameter movs
  insertParameterMovs();
  
  // Insert prolog
  insertProlog();
  
  // Code generation
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
  for (auto pair : *blocks) {
    auto bl = pair.second;
    bl->finalize();
  }
}



void GraphAssembler::phiInsertion()
{
  for (auto const& l : *labels) {
    auto lb = blocks->at(l);
    
    // Step 2: Load all phis into temporary helper registers
    for (auto const& phi : lb->phis) {
      auto bl = get_nodes_block(phi);
      
      for (int i = 0; i < get_Phi_n_preds(phi); i++) {
        ir_node *phipred = get_Phi_pred(phi, i);
        auto inReg = getValue(phipred);
        auto helper = getValue(phi);
        
        ir_node *j = get_Block_cfgpred(bl, i);
        ir_node *jbl = get_nodes_block(j);
        
        Label l = getLabel(jbl);
        shared_ptr<LabeledBlock> lb = blocks->at(l);
        
        auto m = make_shared<mov>(__func__, __LINE__); TODO:
        m->src1 = inReg;
        m->dest = helper;
        
        lb->instructions.push_back(m);
      }
    }
  }
}


string GraphAssembler::irgCodeGeneration()
{
  // call generate() for every instruction, set labels, create one long string
  std::string assembler;
  
  auto ldname = std::string(get_entity_ld_name(get_irg_entity(irg)));
  
  assembler += ".p2align 4,,15\n";
#ifdef __APPLE__
#elif __linux__
  assembler += ".type " + ldname + ", @function\n";
#endif
  
  for (auto const& label : *labels) {
    assembler += label + ":\n";
    
    auto bl = blocks->at(label);
    auto instructions = bl->instructions;
    
    for (auto const& instruction : instructions) {
      assembler += "\t" + instruction->generate() + "\n";
    }
  }
  
#ifdef __APPLE__
#elif __linux__
  assembler += ".size " + ldname + ", .-" + ldname + "\n\n";
#endif
  
  return move(assembler);
}
