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

shared_ptr<Register> GraphAssembler::getRegister(ir_node *node) {
  if (registers.count(get_irn_node_nr(node)) > 0) {
    return registers.at(get_irn_node_nr(node));
  }
  
  auto r = make_shared<Register>(get_irn_mode(node));
  registers.emplace(get_irn_node_nr(node), r);
  return r;
}

long GraphAssembler::getStackOffsetForRegister(shared_ptr<Register> r) {
  if (registerToStackOffset.count(r) > 0) {
    return registerToStackOffset.at(r);
  }
  
  if (r->size == RegisterSize32) {
    topOfStack -= 4;
  } else {
    topOfStack -= 8;
  }
  
  registerToStackOffset.emplace(r, topOfStack);
  return topOfStack;
}

shared_ptr<Register> GraphAssembler::getHelperRegister(ir_node *node) {
  if (helperRegisters.count(get_irn_node_nr(node)) > 0) {
    return helperRegisters.at(get_irn_node_nr(node));
  }
  
  auto r = make_shared<Register>(get_irn_mode(node));
  helperRegisters.emplace(get_irn_node_nr(node), r);
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




#pragma mark - Function prolog/epilog

void GraphAssembler::insertProlog() {
  auto p = make_shared<push>();
  p->src1 = Register::rbp();
  
  auto m = make_shared<mov>();
  m->src1 = Register::rsp();
  m->dest = Register::rbp();
  
  auto s = make_shared<subq_rsp>();
  s->nslots = (unsigned) Register::numberOfDynamicRegisters();
  
  Label fl = labels.front();
  auto bl = blocks.at(fl);
  auto it = bl->instructions.begin();
  bl->instructions.insert(it, p);
  it = bl->instructions.begin();
  bl->instructions.insert(it + 1, m);
  it = bl->instructions.begin();
  bl->instructions.insert(it + 2, s);

  // rename first label to function name
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
  getRegister(node);
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
  long l = get_tarval_long(val);
  auto m = make_shared<mov_from_imm>();
  m->imm_value = l;
  auto oreg = getRegister(node);
  m->dest = oreg;
  
  getLabeledBlockForIrNode(node)->instructions.push_back(m);
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
  
  auto comp = make_shared<cmp>();
  comp->src1 = lreg;
  comp->src2 = rreg;
//  getCurrentBlock()->instructions.push_back(cmp);
  getLabeledBlockForIrNode(node)->exitInstructions.push_back(comp);
  
  auto br = make_shared<Branch>();
  br->relation = get_Cmp_relation(s);
  br->label = trueLabel;
//  getCurrentBlock()->instructions.push_back(br);
  getLabeledBlockForIrNode(node)->exitInstructions.push_back(br);
  
  auto j = make_shared<jmp>();
  j->label = falseLabel;
//  getCurrentBlock()->instructions.push_back(j);
  getLabeledBlockForIrNode(node)->exitInstructions.push_back(j);
}

void GraphAssembler::buildJmp(ir_node *node) {
  foreach_out_edge_safe(node, edge) {
    ir_node *n = get_edge_src_irn(edge);
    assert(is_Block(n));
    Label l = getLabel(n);
    
    auto j = make_shared<jmp>();
    j->label = l;
//    getCurrentBlock()->instructions.push_back(j);
    
    getLabeledBlockForIrNode(node)->exitInstructions.push_back(j);
    return;
  }
}

void GraphAssembler::buildProj(ir_node *node) {
  ir_node *pred = get_Proj_pred(node);
  if (is_Proj(pred)) {
    ir_node *ppred = get_Proj_pred(pred);
    if (is_Start(ppred)) {
      // Is an argument
      auto m = make_shared<mov_from_stack>();
      m->offset = get_Proj_num(node);
      auto oreg = getRegister(node);
      m->dest = oreg;
      getLabeledBlockForIrNode(node)->instructions.push_back(m);
    }
  }
}

void GraphAssembler::buildAdd(ir_node *node) {
  ir_node *l = get_Add_left(node);
  ir_node *r = get_Add_right(node);
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];
  auto oreg = getRegister(node);
  
  auto inst = make_shared<add>();
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;

  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}

void GraphAssembler::buildReturn(ir_node *node) {
  if (get_Return_n_ress(node) > 0) {
    ir_node *pred = get_Return_res(node, 0);
    auto r = registers[get_irn_node_nr(pred)];
    auto inst = make_shared<mov>();
    inst->src1 = r;
    inst->dest = Register::_ax(inst->src1->size);
    getLabeledBlockForIrNode(node)->instructions.push_back(inst);
  }
  
  auto po = make_shared<pop>();
  po->dest = Register::rbp();
  getLabeledBlockForIrNode(node)->instructions.push_back(po);
  auto ret = make_shared<retq>();
  getLabeledBlockForIrNode(node)->instructions.push_back(ret);
}

void GraphAssembler::buildCall(ir_node *node) {
	ir_entity *e = get_Call_callee(node);
  
	// use standardized x86_64 calling convention: 
	// http://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64
	// "According to the ABI, the first 6 integer or pointer arguments to a function are passed in registers."
	
	int paramsNum = get_Call_n_params(node);
	
	// iterate over first 6 parameters for the registers (from back)
	for (int i = 0; i < 6 && i < paramsNum; i++)
	{
		ir_node *a = get_Call_param(node, i);
		
		assert(registers.count(get_irn_node_nr(a)) > 0);
		shared_ptr<Register> reg = registers[get_irn_node_nr(a)];
		
		// map each parameter to register according to ABI
		long identifier;
		switch (i)
		{
			case 0:
				identifier = 7;
				break;
			
			case 1:
				identifier = 6;
				break;
				
			case 2:
				identifier = 3;
				break;
			
			case 3:
				identifier = 2;
				break;
			
			case 4:
				identifier = 8;
				break;
			
			case 5:
				identifier = 9;
				break;
			
			default:
				assert(false);
		}
		
		if (reg->type == RegisterTypeVirtual)
		{
			// if the register is still virtual, we can set it to a physical one
			reg->type = RegisterTypePhysical;
			reg->identifier = identifier;
		}
		else
		{
			// if the register is already physical, we have to insert a move instruction
			// TODO: this is dependend from the register allocator!
			// TODO: we insert a move without checking if the dest-register is currently used for any other variable
			auto r = make_shared<Register>();
			r->size = reg->size;
			r->type = RegisterTypePhysical;
			r->identifier = identifier;
			
			auto m = make_shared<mov>();
			m->src1 = reg;
			m->dest = r;
			getLabeledBlockForIrNode(node)->instructions.push_back(m);
		}
	}
	
	// iterate over the remaining parameters for the stack
	for (int i = paramsNum - 1; i > 5 && i < paramsNum; i++)
	{
		ir_node *a = get_Call_param(node, i);
		
		assert(registers.count(get_irn_node_nr(a)) > 0);
		shared_ptr<Register> reg = registers[get_irn_node_nr(a)];
		
		// create mov instruction to move value to stack
		auto m = make_shared<push>();
		m->src1 = reg;
		getLabeledBlockForIrNode(node)->instructions.push_back(m);
	}
	
	// align base pointer to 2^8
	
	//getLabeledBlockForIrNode(node)->instructions.push_back(make_shared<popq_rbp>());
}

void GraphAssembler::buildSub(ir_node *node) {
  ir_node *l = get_Sub_left(node);
  ir_node *r = get_Sub_right(node);
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];
  auto oreg = getRegister(node);
  
  auto inst = make_shared<sub>();
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
  auto oreg = getRegister(node);
  
  auto inst = make_shared<imul>();
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
  auto oreg = getRegister(node);
  
  auto inst = make_shared<idiv>();
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
  //TODO: read result from %eax
}

void GraphAssembler::buildMod(ir_node *node) {
  ir_node *l = get_Mod_left(node);
  ir_node *r = get_Mod_right(node);
  
  auto lreg = registers[get_irn_node_nr(l)];
  auto rreg = registers[get_irn_node_nr(r)];
  auto oreg = getRegister(node);
  
  auto inst = make_shared<idiv>();
  inst->src1 = lreg;
  inst->src2 = rreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
  //TODO: read result from %edx
}

void GraphAssembler::buildMinus(ir_node *node) {
  ir_node *m = get_Minus_op(node);

  auto mreg = registers[get_irn_node_nr(m)];
  auto oreg = getRegister(node);
  
  auto inst = make_shared<neg>();
  inst->src1 = mreg;
  inst->dest = oreg;
  getLabeledBlockForIrNode(node)->instructions.push_back(inst);
}





#pragma mark - Physical register allocation

shared_ptr<Instruction> GraphAssembler::getMovFromStackOrPhysicalRegister(shared_ptr<Register> from, shared_ptr<Register> to) {
  if (from->type == RegisterTypePhysical) {
    auto m1 = make_shared<mov>();
    m1->src1 = from;
    m1->dest = to;
    return m1;
  }
  
  auto m1 = make_shared<mov_from_stack>();
  m1->offset = getStackOffsetForRegister(from);
  m1->dest = to;
  
  return m1;
}


shared_ptr<Instruction> GraphAssembler::getMovToStackOrPhysicalRegister(shared_ptr<Register> from, shared_ptr<Register> to) {
  if (to->type == RegisterTypePhysical) {
    auto m1 = make_shared<mov>();
    m1->src1 = from;
    m1->dest = to;
    return m1;
  }
  
  auto m1 = make_shared<mov_to_stack>();
  m1->offset = getStackOffsetForRegister(to);
  m1->src = from;
  
  return m1;
}


void GraphAssembler::allocI2to1(shared_ptr<Instruction> instr, I2to1 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  // load arg1 to _ax
  auto ax = Register::_ax(i->src1->size);
  auto m1 = getMovFromStackOrPhysicalRegister(i->src1, ax);

  // load arg2 to _bx
  auto bx = Register::_bx(i->src2->size);
  auto m2 = getMovFromStackOrPhysicalRegister(i->src2, bx);
  
  // store dest
  auto m3 = getMovToStackOrPhysicalRegister(bx, i->dest);

  i->src1 = ax;
  i->src2 = bx;
  i->dest = i->src2;
  
  instructions_.push_back(m1);
  instructions_.push_back(m2);
  instructions_.push_back(instr);
  instructions_.push_back(m3);
}

void GraphAssembler::allocI2to0(shared_ptr<Instruction> instr, I2to0 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  // load arg1 to _ax
  auto ax = Register::_ax(i->src1->size);
  auto m1 = getMovFromStackOrPhysicalRegister(i->src1, ax);
  
  // load arg2 to _bx
  auto bx = Register::_bx(i->src2->size);
  auto m2 = getMovFromStackOrPhysicalRegister(i->src2, bx);
  
  i->src1 = ax;
  i->src2 = bx;
  
  instructions_.push_back(m1);
  instructions_.push_back(m2);
  instructions_.push_back(instr);
}

void GraphAssembler::allocI1to1(shared_ptr<Instruction> instr, I1to1 *i, vector<shared_ptr<Instruction>> &instructions_)
{
  // load arg1 to _ax
  auto ax = Register::_ax(i->src1->size);
  auto m1 = getMovFromStackOrPhysicalRegister(i->src1, ax);
  
  // store dest
  auto bx = Register::_bx(i->dest->size);
  auto m3 = getMovToStackOrPhysicalRegister(bx, i->dest);
  
  i->src1 = ax;
  i->dest = bx;
  
  instructions_.push_back(m1);
  instructions_.push_back(instr);
  instructions_.push_back(m3);
}

void GraphAssembler::allocI1to0(shared_ptr<Instruction> instr, I1to0 *i, vector<shared_ptr<Instruction>> &instructions_)
{ 
  // load arg1 to _ax
  auto ax = Register::_ax(i->src1->size);
  auto m1 = getMovFromStackOrPhysicalRegister(i->src1, ax);

  i->src1 = ax;

  instructions_.push_back(m1);
  instructions_.push_back(instr);
}

void GraphAssembler::allocMoveFromStack(shared_ptr<Instruction> instr, mov_from_stack *i, vector<shared_ptr<Instruction>> &instructions_)
{
//  // save result to stack
//  auto m = make_shared<movl_to_stack>();
//  m->offset = getStackOffsetForRegister(i->dest);
//  i->dest = Register::_ax(i->dest->size);
//  m->src = i->dest;
//
//  instructions_.push_back(instr);
//  instructions_.push_back(m);
}

void GraphAssembler::allocMoveFromImm(shared_ptr<Instruction> instr, mov_from_imm *i, vector<shared_ptr<Instruction>> &instructions_)
{
//  // save result to stack
//  auto m = make_shared<movl_to_stack>();
//  m->offset = getStackOffsetForRegister(i->dest);
//  i->dest = Register::_ax(i->dest->size);
//  m->src = i->dest;
//
//  instructions_.push_back(instr);
//  instructions_.push_back(m);
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
    _this->collectPhi(node);
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

  if (is_Call(node)) {
    _this->buildCall(node);
  }
    
  if (is_Sub(node)) {
    _this->buildSub(node);
  }
  
  if (is_Mul(node)) {
    _this->buildMul(node);
  }
 
  if (is_Div(node)) {
    _this->buildDiv(node);
  }

  if (is_Mod(node)) {
    _this->buildMod(node);
  } 
 
  if (is_Minus(node)) {
    _this->buildMinus(node);
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
        auto inReg = registers.at(get_irn_node_nr(phipred));
        auto helper = getHelperRegister(phipred);
        
        ir_node *j = get_Block_cfgpred(bl, i);
        ir_node *jbl = get_nodes_block(j);
        
        Label l = nodeNrToLabel.at(get_irn_node_nr(jbl));
        shared_ptr<LabeledBlock> lb = blocks.at(l);
        
        auto m = make_shared<mov>();
        m->src1 = inReg;
        m->dest = helper;
        
        lb->instructions.push_back(m);
      }
    }
    
    // Step 3: Load all phis from temporary helper to out register
    for (auto const& phi : lb->phis) {
      auto bl = get_nodes_block(phi);
      auto outReg = getRegister(phi);
      
      for (int i = 0; i < get_Phi_n_preds(phi); i++) {
        ir_node *phipred = get_Phi_pred(phi, i);
        auto helper = getHelperRegister(phipred);
        
        ir_node *j = get_Block_cfgpred(bl, i);
        ir_node *jbl = get_nodes_block(j);
        
        Label l = nodeNrToLabel.at(get_irn_node_nr(jbl));
        shared_ptr<LabeledBlock> lb = blocks.at(l);
        
        auto m = make_shared<mov>();
        m->src1 = helper;
        m->dest = outReg;
        
        lb->instructions.push_back(m);
      }
    }
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
      } else if (auto i = dynamic_cast<I2to0*>(instruction.get())) {
        allocI2to0(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<I1to1*>(instruction.get())) {
        allocI1to1(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<I1to0*>(instruction.get())) {
        allocI1to0(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<mov_from_stack*>(instruction.get())) {
        allocMoveFromStack(instruction, i, instructions_);
      } else if (auto i = dynamic_cast<mov_from_imm*>(instruction.get())) {
        allocMoveFromImm(instruction, i, instructions_);
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
  
  for (auto const& label : labels) {
    assembler += label + ":\n";
    
    auto instructions = blocks.at(label)->instructions;
    
    for (auto const& instruction : instructions) {
      assembler += "\t" + instruction->generate() + "\n";
    }
  }
  
  return move(assembler);
}
