#include "irbuilder.h"
#include <assert.h>
#include "../stringtable/stringtable.h"
#include <libfirm/firm.h>
#include <iostream>
#include "types.h"


using namespace cmpl;



#pragma mark Expression::assign

void CRef::assign(ir_node *value) {
  auto d = shared_from_this()->declaration.lock();
  
  if (auto f = dynamic_cast<Field*>(d.get())) {
    // Field Access
    ir_graph *g   = get_current_ir_graph();
    ir_node *args = get_irg_args(g);
    ir_node *th   = new_Proj(args, mode_P, 0);
    ir_node *irn  = new_Member(th, f->firm_entity);
    ir_node *st   = new_Store(get_store(), irn, value, f->type->getFirmType(), cons_none);
    ir_node *m    = new_Proj(st, mode_M, pn_Store_M);
    set_store(m);
  } else {
    // Local Variable
    set_value(d->parameterIndex, value);
  }
}

void UnaryRightExpression::assign(ir_node *value) {
  auto n = shared_from_this();
  n->expression->doExpr();
  
  if (auto fa = dynamic_cast<FieldAccess*>(n->op.get())) {
    auto decl = fa->declaration.lock();
    assert(decl);
    
    assert(n->expression->firm_node);
    assert(decl->firm_entity);
    assert(decl->type->getFirmType());
    
    ir_node *irn = new_Member(n->expression->firm_node, decl->firm_entity);
    ir_node *st = new_Store(get_store(), irn, value, decl->type->getFirmType(), cons_none);
    ir_node *m   = new_Proj(st, mode_M, pn_Store_M);
    set_store(m);
    
    return;
  }
  
  if (auto aa = dynamic_cast<ArrayAccess*>(n->op.get())) {
    assert(n->expression->type->getFirmType());
    aa->expression->doExpr();
    
    ir_type *array_type = get_pointer_points_to_type(n->expression->type->getFirmType());
    ir_type *elem_type  = get_array_element_type(array_type);
    ir_node *sel        = new_Sel(n->expression->firm_node, aa->expression->firm_node, array_type);
    ir_node *st         = new_Store(get_store(), sel, value, elem_type, cons_none);
    ir_node *m          = new_Proj(st, mode_M, pn_Load_M);
    set_store(m);
    
    return;
  }
  
  assert(false);
}




#pragma mark - doExpr / doCond

void AssignmentExpression::doExpr() {
  // In this case the assignment is not embedded in a branching context
  // It must return a value
  // Example 1: x = (a = ...)
  //          this: ---------
  
  auto n = shared_from_this();
  n->expression2->doExpr();
  n->expression1->assign(n->expression2->firm_node);
  n->firm_node = n->expression2->firm_node;
}

void AssignmentExpression::doCond(ir_node *trueBlock, ir_node *falseBlock) {
  // In this case the assignment is already embedded in a branching context
  // It need not return a value, but lhs (boolean) must be assigned the value of rhs (boolean)
  // The assignment acts like an equality check for 1 (true) or 0 (false)
  // Example 1: if (a = ...) {}
  //         this: ---------
  // Example 2: x = (a = ...) || ();
  //          this: ---------
  
  shared_from_this()->expression2->doExpr();
  assert(shared_from_this()->expression2->firm_node);
  
  shared_from_this()->expression1->assign(shared_from_this()->expression2->firm_node);
  
  ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
  ir_node *cmp = new_Cmp(shared_from_this()->expression2->firm_node, const1, ir_relation_greater_equal);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
}

void LogicalOrExpression::doExpr() {
  ir_graph *g = get_current_ir_graph();
  ir_node *trueBlock = new_r_immBlock(g);
  ir_node *falseBlock = new_r_immBlock(g);
  ir_node *nextBlock = new_r_immBlock(g);
  
  doCond(trueBlock, falseBlock);
  
  mature_immBlock(get_cur_block());
  
  set_cur_block(trueBlock);
  ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
  ir_node *jmpTrue = new_Jmp();
  
  set_cur_block(falseBlock);
  ir_node *const0 = new_Const(new_tarval_from_long(0, mode_Bu));
  ir_node *jmpFalse = new_Jmp();
  
  add_immBlock_pred(nextBlock, jmpTrue);
  mature_immBlock(trueBlock);
  add_immBlock_pred(nextBlock, jmpFalse);
  mature_immBlock(falseBlock);
  
  set_cur_block(nextBlock);
  
  ir_node *results[2] = { const1, const0 };
  ir_node *phi = new_Phi(2, results, mode_Bu);
  shared_from_this()->firm_node = phi;
}

void LogicalOrExpression::doCond(ir_node *trueBlock, ir_node *falseBlock) {
  ir_graph *g = get_current_ir_graph();
  
  ir_node *rightBlock = new_r_immBlock(g);
  shared_from_this()->expression1->doCond(trueBlock, rightBlock);
  mature_immBlock(rightBlock);
  
  set_cur_block(rightBlock);
  shared_from_this()->expression2->doCond(trueBlock, falseBlock);
}

void LogicalAndExpression::doExpr() {
  ir_graph *g = get_current_ir_graph();
  ir_node *trueBlock = new_r_immBlock(g);
  ir_node *falseBlock = new_r_immBlock(g);
  ir_node *nextBlock = new_r_immBlock(g);
  
  doCond(trueBlock, falseBlock);
  
  mature_immBlock(get_cur_block());
  
  set_cur_block(trueBlock);
  ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
  ir_node *jmpTrue = new_Jmp();
  
  set_cur_block(falseBlock);
  ir_node *const0 = new_Const(new_tarval_from_long(0, mode_Bu));
  ir_node *jmpFalse = new_Jmp();
  
  add_immBlock_pred(nextBlock, jmpTrue);
  mature_immBlock(trueBlock);
  add_immBlock_pred(nextBlock, jmpFalse);
  mature_immBlock(falseBlock);
  
  set_cur_block(nextBlock);
  
  ir_node *results[2] = { const1, const0 };
  ir_node *phi = new_Phi(2, results, mode_Bu);
  shared_from_this()->firm_node = phi;
}

void LogicalAndExpression::doCond(ir_node *trueBlock, ir_node *falseBlock) {
  ir_graph *g = get_current_ir_graph();
  
  ir_node *leftBlock = new_r_immBlock(g);
  shared_from_this()->expression1->doCond(leftBlock, falseBlock);
  mature_immBlock(leftBlock);
  
  set_cur_block(leftBlock);
  shared_from_this()->expression2->doCond(trueBlock, falseBlock);
}

void EqualityExpression::doExpr() {
  ir_graph *g = get_current_ir_graph();
  ir_node *trueBlock = new_r_immBlock(g);
  ir_node *falseBlock = new_r_immBlock(g);
  ir_node *nextBlock = new_r_immBlock(g);
  
  doCond(trueBlock, falseBlock);
  
  mature_immBlock(get_cur_block());
  
  set_cur_block(trueBlock);
  ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
  ir_node *jmpTrue = new_Jmp();
  
  set_cur_block(falseBlock);
  ir_node *const0 = new_Const(new_tarval_from_long(0, mode_Bu));
  ir_node *jmpFalse = new_Jmp();
  
  add_immBlock_pred(nextBlock, jmpTrue);
  mature_immBlock(trueBlock);
  add_immBlock_pred(nextBlock, jmpFalse);
  mature_immBlock(falseBlock);

  set_cur_block(nextBlock);
  
  ir_node *results[2] = { const1, const0 };
  ir_node *phi = new_Phi(2, results, mode_Bu);
  shared_from_this()->firm_node = phi;
}

void EqualityExpression::doCond(ir_node *trueBlock, ir_node *falseBlock) {
  shared_from_this()->expression1->doExpr();
  shared_from_this()->expression2->doExpr();
  
  ir_relation rel = ir_relation_false;
  
  if (dynamic_cast<Equals*>(shared_from_this()->op.get())) {
    rel = ir_relation_equal;
  }
  
  if (dynamic_cast<NotEquals*>(shared_from_this()->op.get())) {
    rel = ir_relation_less_greater;
  }
  
  ir_node *cmp = new_Cmp(shared_from_this()->expression1->firm_node, shared_from_this()->expression2->firm_node, rel);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
}

void RelationalExpression::doExpr() {
  //  assert(false);
  ir_graph *g = get_current_ir_graph();
  ir_node *trueBlock = new_r_immBlock(g);
  ir_node *falseBlock = new_r_immBlock(g);
  ir_node *nextBlock = new_r_immBlock(g);
  
  doCond(trueBlock, falseBlock);
  
  mature_immBlock(get_cur_block());
  
  set_cur_block(trueBlock);
  ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
  ir_node *jmpTrue = new_Jmp();
  
  set_cur_block(falseBlock);
  ir_node *const0 = new_Const(new_tarval_from_long(0, mode_Bu));
  ir_node *jmpFalse = new_Jmp();
  
  add_immBlock_pred(nextBlock, jmpTrue);
  mature_immBlock(trueBlock);
  add_immBlock_pred(nextBlock, jmpFalse);
  mature_immBlock(falseBlock);
  
  set_cur_block(nextBlock);
  
  ir_node *results[2] = { const1, const0 };
  ir_node *phi = new_Phi(2, results, mode_Bu);
  shared_from_this()->firm_node = phi;
}

void RelationalExpression::doCond(ir_node *trueBlock, ir_node *falseBlock) {
  shared_from_this()->expression1->doExpr();
  shared_from_this()->expression2->doExpr();
  
  ir_relation rel = ir_relation_false;
  
  if (dynamic_cast<LessThan*>(shared_from_this()->op.get())) {
    rel = ir_relation_less;
  }
  
  if (dynamic_cast<LessThanOrEqual*>(shared_from_this()->op.get())) {
    rel = ir_relation_less_equal;
  }
  
  if (dynamic_cast<GreaterThan*>(shared_from_this()->op.get())) {
    rel = ir_relation_greater;
  }
  
  if (dynamic_cast<GreaterThanOrEqual*>(shared_from_this()->op.get())) {
    rel = ir_relation_greater_equal;
  }
  
  ir_node *cmp = new_Cmp(shared_from_this()->expression1->firm_node, shared_from_this()->expression2->firm_node, rel);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
}

void AdditiveExpression::doExpr() {
  auto n = shared_from_this();
  n->expression1->doExpr();
  n->expression2->doExpr();
  
  if (dynamic_cast<Add*>(n->op.get())) {
    n->firm_node = new_Add(n->expression1->firm_node, n->expression2->firm_node);
    return;
  }
  
  if (dynamic_cast<Subtract*>(shared_from_this()->op.get())) {
    n->firm_node = new_Sub(n->expression1->firm_node, n->expression2->firm_node);
    return;
  }
  
  assert(false);
}

void MultiplicativeExpression::doExpr() {
  auto n = shared_from_this();
  n->expression1->doExpr();
  n->expression2->doExpr();
  
  if (dynamic_cast<Multiply*>(n->op.get())) {
    n->firm_node = new_Mul(n->expression1->firm_node, n->expression2->firm_node);
    return;
  }
  
  if (dynamic_cast<Divide*>(n->op.get())) {
    ir_node *mem = get_store();
    ir_node* div = new_Div(mem, n->expression1->firm_node, n->expression2->firm_node, op_pin_state_pinned);
    n->firm_node = new_Proj(div, mode_Is, pn_Div_res);
    set_store(mem);
    return;
  }
  
  if (dynamic_cast<Modulo*>(n->op.get())) {
    ir_node *mem = get_store();
    ir_node* div = new_Mod(mem, n->expression1->firm_node, n->expression2->firm_node, op_pin_state_pinned);
    n->firm_node = new_Proj(div, mode_Is, pn_Mod_res);
    mem = new_Proj(div, mode_M, pn_Mod_M);
    set_store(mem);
    return;
  }
  
  assert(false);
}

void UnaryLeftExpression::doExpr() {
  auto n = shared_from_this();
  
  if (dynamic_cast<Minus*>(n->op.get())) {
    n->expression->doExpr();
    n->firm_node = new_Minus(n->expression->firm_node);
    return;
  }
  
  if (dynamic_cast<Negate*>(n->op.get())) {
    ir_graph *g = get_current_ir_graph();
    ir_node *trueBlock = new_r_immBlock(g);
    ir_node *falseBlock = new_r_immBlock(g);
    ir_node *nextBlock = new_r_immBlock(g);
    
    doCond(trueBlock, falseBlock);
    
    mature_immBlock(get_cur_block());
    
    set_cur_block(trueBlock);
    ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
    ir_node *jmpTrue = new_Jmp();
    
    set_cur_block(falseBlock);
    ir_node *const0 = new_Const(new_tarval_from_long(0, mode_Bu));
    ir_node *jmpFalse = new_Jmp();
    
    add_immBlock_pred(nextBlock, jmpTrue);
    mature_immBlock(trueBlock);
    add_immBlock_pred(nextBlock, jmpFalse);
    mature_immBlock(falseBlock);
    
    set_cur_block(nextBlock);
    
    ir_node *results[2] = { const1, const0 };
    ir_node *phi = new_Phi(2, results, mode_Bu);
    shared_from_this()->firm_node = phi;
    return;
  }
  
  assert(false);
}

void UnaryLeftExpression::doCond(ir_node *trueBlock, ir_node *falseBlock) {
  auto n = shared_from_this();
  
  n->expression->doCond(falseBlock, trueBlock);
}

void CRef::doExpr() {
  auto d = shared_from_this()->declaration.lock();
  assert(d);
  
  ir_node *res = NULL;
  
  if (auto decl = dynamic_cast<Field*>(d.get())) {
    // is field access
    ir_graph *g = get_current_ir_graph();
    ir_node *args = get_irg_args(g);
    ir_node *this_node = new_Proj(args, mode_P, 0);
    
    ir_node *irn = new_Member(this_node, decl->firm_entity);
    ir_mode *mode = get_type_mode(decl->type->getFirmType());
    ir_node *ld  = new_Load(get_store(), irn, mode, decl->type->getFirmType(), cons_none);
    ir_node *m   = new_Proj(ld, mode_M, pn_Load_M);
    res = new_Proj(ld, mode, pn_Load_res);
   
    set_store(m);
  } else {
    // is local var access
    if (auto decl = dynamic_cast<TypedNode*>(d.get())) {
      res = get_value(d->parameterIndex, get_type_mode(decl->type->getFirmType()));
    } else {
      assert(false);
    }
  }
  
  shared_from_this()->firm_node = res;
}

void StaticLibraryCallExpression::doExpr()
{
  auto n = shared_from_this();
  
  ir_entity *meth = n->getFirmEntity();
  
  // println() only has one argument (no this-pointer!)
  ir_node *addr = new_Address(meth);
  ir_node *args[1];
  
  n->expression->doExpr();
  assert(n->expression->firm_node);
  
  args[0] = n->expression->firm_node;
  
  ir_node *call = new_Call(get_store(), addr, 1, args, n->getFirmType());
  ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
  ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
  //ir_node *res = new_Proj(tres, Types::getVoidNode()->type->getFirmMode(), 0);
  
  set_store(mem);
  
  // TODO: returntype is void... what do do?
  n->firm_node = call;
}

void NewArray::doExpr()
{
  auto n = shared_from_this();
  n->expression->doExpr();
  
  ir_node *num = n->expression->firm_node;
  ir_node *call = IRBuilder::callCallocNode(num, n->type->getFirmType());
  ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
  ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
  ir_node *res = new_Proj(tres, mode_P, 0);
  
  set_store(mem);
  n->firm_node = res;
}

void CNull::doExpr()
{
  ir_tarval *tv = new_tarval_from_long(0, mode_P);
  shared_from_this()->firm_node = new_Const(tv);
}

void CThis::doExpr()
{
  ir_graph *g = get_current_ir_graph();
  
  ir_node *this_node = new_Proj(get_irg_args(g), mode_P, 0);
  shared_from_this()->firm_node = this_node;
}

void CallExpression::doCond(ir_node *trueBlock, ir_node *falseBlock)
{
  auto n = shared_from_this();
  
  doExpr();
  assert(n->firm_node);
  
  ir_node *res = n->firm_node;
  
  // Check res for trueness
  ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
  ir_node *cmp = new_Cmp(res, const1, ir_relation_greater_equal);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
}

void CallExpression::doExpr()
{
  auto n = shared_from_this();
  
  ir_graph *g = get_current_ir_graph();
  ir_node *this_node = new_Proj(get_irg_args(g), mode_P, 0);
  
  auto decl = n->declaration.lock();
  assert(decl);
  
  ir_entity *meth = decl->firm_entity;
  ir_node *addr = new_Address(meth);
  unsigned long nargs = n->arguments.size();
  ir_node *args[nargs + 1];
  args[0] = this_node;
  int i = 1;
  for (auto const& a : n->arguments) {
    a->doExpr();
    assert(a->firm_node);
    
    args[i++] = a->firm_node;
  }
  
  ir_node *call = new_Call(get_store(), addr, (int)nargs + 1, args, decl->declared_type);
  ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
  set_store(mem);
  
  if (auto t = decl->type->getFirmType()) {
    ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
    ir_mode *mode = get_type_mode(decl->type->getFirmType());
    ir_node *res = new_Proj(tres, mode, 0);
    n->firm_node = res;
  }
}

void UnaryRightExpression::doCond(ir_node *trueBlock, ir_node *falseBlock)
{
  auto n = shared_from_this();
  
  doExpr();
  assert(n->firm_node);
  
  // Check res for trueness
  ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
  ir_node *cmp = new_Cmp(n->firm_node, const1, ir_relation_greater_equal);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
}

void UnaryRightExpression::doExpr()
{
  auto n = shared_from_this();
  
  n->expression->doExpr();
  assert(n->expression->firm_node);
  
  if (FieldAccess* fa = dynamic_cast<FieldAccess*>(n->op.get()))
  {
    auto decl = fa->declaration.lock();
    assert(decl);
    
    // n->expression->firm_node is a Proj P64 to a class entity
    ir_node *irn = new_Member(n->expression->firm_node, decl->firm_entity);
    ir_mode *mode = get_type_mode(decl->type->getFirmType());
    ir_node *ld  = new_Load(get_store(), irn, mode, decl->type->getFirmType(), cons_none);
    ir_node *m   = new_Proj(ld, mode_M, pn_Load_M);
    ir_node *res = new_Proj(ld, mode, pn_Load_res);
    
    set_store(m);
    n->firm_node = res;
    return;
  }
  
  if (MethodInvocation* mi = dynamic_cast<MethodInvocation*>(n->op.get()))
  {
    ir_graph *g = get_current_ir_graph();
    // n->expression->firm_node is a Proj P64 to a class entity
    ir_node *this_node = n->expression->firm_node;
    
    auto decl = mi->declaration.lock();
    assert(decl);
    
    ir_entity *meth = decl->firm_entity;
    ir_node *addr = new_Address(meth);
    unsigned long nargs = mi->arguments.size();
    ir_node *args[nargs + 1];
    args[0] = this_node;
    int i = 1;
    for (auto const& a : mi->arguments) {
      a->doExpr();
      assert(a->firm_node);
      
      args[i++] = a->firm_node;
    }
    
    ir_node *call = new_Call(get_store(), addr, (int)nargs + 1, args, decl->declared_type);
    ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
    set_store(mem);
    
    if (auto t = decl->type->getFirmType()) {
      ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
      ir_mode *mode = get_type_mode(decl->type->getFirmType());
      ir_node *res = new_Proj(tres, mode, 0);
      n->firm_node = res;
    }
  }
  
  if (ArrayAccess* aa = dynamic_cast<ArrayAccess*>(n->op.get()))
  {
    // n->expression->firm_node is a Proj P64 to an array_type
    aa->expression->doExpr();
    assert(aa->expression->firm_node);
    assert(is_Pointer_type(n->expression->type->getFirmType()));
    
    ir_type *array_type = get_pointer_points_to_type(n->expression->type->getFirmType());
    ir_mode *array_mode = get_type_mode(array_type);
    ir_type *elem_type  = get_array_element_type(array_type);
    ir_mode *elem_mode  = get_type_mode(elem_type);
    
    ir_node *sel        = new_Sel(n->expression->firm_node, aa->expression->firm_node, array_type);
    ir_node *ld         = new_Load(get_store(), sel, elem_mode, elem_type, cons_none);
    ir_node *m          = new_Proj(ld, mode_M, pn_Load_M);
    ir_node *res        = new_Proj(ld, elem_mode, pn_Load_res);
    
    set_store(m);
    n->firm_node = res;
  }
}

void NewObject::doExpr() {
  auto n = shared_from_this();
  
  ir_node *num = new_Const(new_tarval_from_long(1, mode_Is));
  ir_node *call = IRBuilder::callCallocNode(num, n->type->getFirmType());
  ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
  ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
  ir_node *res = new_Proj(tres, mode_P, 0);
  
  set_store(mem);
  n->firm_node = res;
}

void CRef::doCond(ir_node *trueBlock, ir_node *falseBlock) {
  auto d = shared_from_this()->declaration.lock();
  assert(d);
  
  ir_node *res = NULL;
  
  if (auto decl = dynamic_cast<Field*>(d.get())) {
    // is field access
    ir_graph *g = get_current_ir_graph();
    ir_node *args = get_irg_args(g);
    ir_node *this_node = new_Proj(args, mode_P, 0);
    
    ir_node *irn = new_Member(this_node, decl->firm_entity);
    ir_mode *mode = get_type_mode(decl->type->getFirmType());
    ir_node *ld  = new_Load(get_store(), irn, mode, decl->type->getFirmType(), cons_none);
    ir_node *m   = new_Proj(ld, mode_M, pn_Load_M);
    res = new_Proj(ld, mode, pn_Load_res);
    
    set_store(m);
  } else {
    // is local var access
    if (auto decl = dynamic_cast<TypedNode*>(d.get())) {
      res = get_value(d->parameterIndex, get_type_mode(decl->type->getFirmType()));
    } else {
      assert(false);
    }
  }
  
  // Check res for trueness
  ir_node *const1 = new_Const(new_tarval_from_long(1, mode_Bu));
  ir_node *cmp = new_Cmp(res, const1, ir_relation_greater_equal);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
}

void CIntegerLiteral::doExpr() {
  ir_tarval *tv = new_tarval_from_long(shared_from_this()->value, mode_Is);
  shared_from_this()->firm_node = new_Const(tv);
}

void CTrue::doExpr() {
  ir_tarval *tv = new_tarval_from_long(1, mode_Bu);
  shared_from_this()->firm_node = new_Const(tv);
}

void CTrue::doCond(ir_node *trueBlock, ir_node *falseBlock) {
//  ir_node *jmp = new_Jmp();
//  add_immBlock_pred(trueBlock, jmp);
//  
//  free(falseBlock);
  
  // Check this->value for trueness
  ir_node *c = new_Const(new_tarval_from_long(0, mode_Bu));
  ir_node *cmp = new_Cmp(c, c, ir_relation_equal);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
}

void CFalse::doExpr() {
  ir_tarval *tv = new_tarval_from_long(0, mode_Bu);
  shared_from_this()->firm_node = new_Const(tv);
}

void CFalse::doCond(ir_node *trueBlock, ir_node *falseBlock) {
//  ir_node *jmp = new_Jmp();
//  add_immBlock_pred(falseBlock, jmp);
//  
//  free(trueBlock);
  
  // Check this->value for trueness
  ir_node *c = new_Const(new_tarval_from_long(0, mode_Bu));
  ir_node *cmp = new_Cmp(c, c, ir_relation_less_greater);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
}







#pragma mark - Helpers

#ifdef __APPLE__
#define CALLOC_NAME "_calloc"
#else
#define CALLOC_NAME "calloc"
#endif

inline void IRBuilder::error(const std::string &err)
{
  throw IRBuilderError(("IRBuilder: " + err).c_str());
}

static ir_entity *calloc_ent = NULL;
static ir_type *calloc_type = NULL;

ir_node *IRBuilder::callCallocNode(ir_node *num, ir_type *result_type) {
  if (!calloc_ent) {
    calloc_type = new_type_method(2, 1, false, cc_cdecl_set, mtp_no_property);
    set_method_res_type(calloc_type, 0, result_type);
    set_method_param_type(calloc_type, 0, new_type_primitive(mode_Is));
    set_method_param_type(calloc_type, 1, new_type_primitive(mode_Is));
    calloc_ent = new_entity(get_glob_type(), new_id_from_str(CALLOC_NAME), calloc_type);
    set_entity_visibility(calloc_ent, ir_visibility_external);
  }
  
  ir_node *addr = new_Address(calloc_ent);
  ir_type *elem_type = get_pointer_points_to_type(result_type);
  int s = get_type_size(elem_type);
  ir_node *size_node = new_Const(new_tarval_from_long(s, mode_Is));
  ir_node *results1[2] = { num, size_node };
  ir_node *call = new_Call(get_store(), addr, 2, results1, calloc_type);
  
  return call;
}





#pragma mark - IRBuilder

void IRBuilder::dispatch(std::shared_ptr<Program> n) {
  for (auto const& c : n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void IRBuilder::dispatch(std::shared_ptr<ClassDeclaration> n) {
  for (auto const& c : n->classMembers) {
    c->accept(shared_from_this());
  }
};

void IRBuilder::dispatch(std::shared_ptr<MainMethod> n) {
  ir_graph *g = n->firm_graph;
  set_current_ir_graph(g);
  set_r_cur_block(g, get_irg_start_block(g));
  
  inMainMethod = true;
  n->block->accept(shared_from_this());
  inMainMethod = false;
  
  
  // create return node with static value 0
  ir_tarval *tv = new_tarval_from_long(0 /* static exit code 0 */, mode_Is);
  ir_node *results[1] = { new_Const(tv) };
  ir_node *ret = new_Return(get_store(), 1, results);
  
  ir_node *end = get_irg_end_block(g);
  add_immBlock_pred(end, ret);
  mature_immBlock(get_r_cur_block(g));
  
  irg_finalize_cons(g);
};

void IRBuilder::dispatch(std::shared_ptr<Field> n) {};

void IRBuilder::dispatch(std::shared_ptr<Method> n) {
  ir_graph *g = n->firm_graph;
  set_current_ir_graph(g);
  set_r_cur_block(g, get_irg_start_block(g));
  ir_node *args = get_irg_args(g);
  
  int i = 1;
  for (auto const& p : n->parameters) {
    ir_mode *mode = get_type_mode(p->type->getFirmType());
    ir_node *proj = new_Proj(args, mode, i++);
    set_value(p->parameterIndex, proj);
  }
  
  n->block->accept(shared_from_this());
  
  if (n->type->equals(Types::getVoidNode())) {
    ir_node *ret = new_Return(get_store(), 0, NULL);
    
    ir_node *end = get_irg_end_block(g);
    add_immBlock_pred(end, ret);
    mature_immBlock(get_r_cur_block(g));
  }
  
  irg_finalize_cons(g);
};

void IRBuilder::dispatch(std::shared_ptr<Parameter> n) { };





#pragma mark - Statements

void IRBuilder::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s : n->statements) {
    s->accept(shared_from_this());
    
    if (s->returns) {
      break;
    }
  }
};

void IRBuilder::dispatch(std::shared_ptr<IfStatement> n) {
  ir_graph *g = get_current_ir_graph();
  
  ir_node *currentBlock = get_cur_block();
  ir_node *trueBlock = new_r_immBlock(g);
  ir_node *falseBlock = new_r_immBlock(g);
  ir_node *nextBlock = new_r_immBlock(g);
  
  n->expression->doCond(trueBlock, falseBlock);
  mature_immBlock(currentBlock);
  
  
  // Handle IF branch
  set_cur_block(trueBlock);
  n->ifStatement->accept(shared_from_this()); // might set_cur_block()
  
  ir_node *jmpIf = new_Jmp();
  
  if (!n->ifStatement->returns) {
    add_immBlock_pred(nextBlock, jmpIf);
    mature_immBlock(get_cur_block());
  }
  
  // Handle ELSE branch
  set_cur_block(falseBlock);
  ir_node *jmpElse = new_Jmp();
  
  add_immBlock_pred(nextBlock, jmpElse);
  mature_immBlock(get_cur_block());
  
  set_cur_block(nextBlock);
};

void IRBuilder::dispatch(std::shared_ptr<IfElseStatement> n) {
  ir_graph *g = get_current_ir_graph();
  
  ir_node *currentBlock = get_cur_block();
  ir_node *trueBlock = new_r_immBlock(g);
  ir_node *falseBlock = new_r_immBlock(g);
  ir_node *nextBlock = new_r_immBlock(g);
  
  n->expression->doCond(trueBlock, falseBlock);
  mature_immBlock(currentBlock);
  
  set_cur_block(trueBlock);
  n->ifStatement->accept(shared_from_this());
  ir_node *jmpIf = new_Jmp();
  
  if (!n->ifStatement->returns) {
    add_immBlock_pred(nextBlock, jmpIf);
    mature_immBlock(get_cur_block());
  }
  
  set_cur_block(falseBlock);
  n->elseStatement->accept(shared_from_this());
  ir_node *jmpElse = new_Jmp();
  
  if (!n->elseStatement->returns) {
    add_immBlock_pred(nextBlock, jmpElse);
    mature_immBlock(get_cur_block());
  }
  
  set_cur_block(nextBlock);
};

void IRBuilder::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->doExpr();
};

void IRBuilder::dispatch(std::shared_ptr<WhileStatement> n) {
  ir_graph *g = get_current_ir_graph();
  
  ir_node *currentBlock = get_cur_block();
  ir_node *headerBlock = new_r_immBlock(g);
  ir_node *bodyBlock = new_r_immBlock(g);
  ir_node *nextBlock = new_r_immBlock(g);
  
  ir_node *jmp1 = new_Jmp();
  mature_immBlock(currentBlock);
  
  set_cur_block(headerBlock);
  // Ensure the creation of a PhiM nodes in case of endless loops
  ir_node *mem = get_store();
  n->expression->doCond(bodyBlock, nextBlock);
  
  set_cur_block(bodyBlock);
  n->statement->accept(shared_from_this());
  ir_node *jmp2 = new_Jmp();
  
  if (!n->statement->returns) {
    add_immBlock_pred(headerBlock, jmp2);
    mature_immBlock(get_cur_block());
  }
  
  add_immBlock_pred(headerBlock, jmp1);
  mature_immBlock(headerBlock);
  
  set_cur_block(nextBlock);
  
  // Keep block in case of endless loops
  keep_alive(headerBlock);
};

void IRBuilder::dispatch(std::shared_ptr<EmptyStatement> n) { };

void IRBuilder::dispatch(std::shared_ptr<ReturnStatement> n) {
  // create "return 0" in case of main method
  ir_graph *g = get_current_ir_graph();
  
  if (inMainMethod)
  {
    // create return node with static value 0
    ir_tarval *tv = new_tarval_from_long(0 /* static exit code 0 */, mode_Is);
    ir_node *results[1] = { new_Const(tv) };
    ir_node *ret = new_Return(get_store(), 1, results);
    
    ir_node *end = get_irg_end_block(g);
    add_immBlock_pred(end, ret);
    mature_immBlock(get_r_cur_block(g));
  }
  else
  {
    // create void return node
    ir_node *ret = new_Return(get_store(), 0, NULL);
    
    ir_node *end = get_irg_end_block(g);
    add_immBlock_pred(end, ret);
    mature_immBlock(get_r_cur_block(g));
  }
};

void IRBuilder::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  ir_graph *g = get_current_ir_graph();
  
  n->expression->doExpr();
  
  ir_node *results[1] = { n->expression->firm_node };
  n->firm_node = new_Return(get_store(), 1, results);
  add_immBlock_pred(get_irg_end_block(g), n->firm_node);
  mature_immBlock(get_r_cur_block(g));
};


#pragma mark - Local variables

void IRBuilder::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  
};

void IRBuilder::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->expression->doExpr();
  set_value(n->parameterIndex, n->expression->firm_node);
};




#pragma mark - Types

void IRBuilder::dispatch(std::shared_ptr<Type> n) { };
void IRBuilder::dispatch(std::shared_ptr<FakeType> n) { };
void IRBuilder::dispatch(std::shared_ptr<NullType> n) { };
void IRBuilder::dispatch(std::shared_ptr<UserType> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeInt> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeBoolean> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeVoid> n) { };







#pragma mark - Expressions

// Expressions are created with expression->doExpr()
// so these methods get never called
// this is necessary because the AST method has to call doCond()
// and doesn't own a reference for this dispatcher
void IRBuilder::dispatch(std::shared_ptr<AssignmentExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<LogicalOrExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<LogicalAndExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<EqualityExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<RelationalExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<AdditiveExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<MultiplicativeExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<UnaryLeftExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<UnaryRightExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<FieldAccess> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<MethodInvocation> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<CallExpression> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<NewObject> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<ArrayAccess> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<CRef> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<NewArray> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<CIntegerLiteral> n) { assert(false); };
void IRBuilder::dispatch(std::shared_ptr<StaticLibraryCallExpression> n) { assert(false); };



#pragma mark - Ops

void IRBuilder::dispatch(std::shared_ptr<Equals> n) { n->firm_relation = ir_relation_equal; };
void IRBuilder::dispatch(std::shared_ptr<NotEquals> n) { n->firm_relation = ir_relation_less_greater; };
void IRBuilder::dispatch(std::shared_ptr<LessThan> n) { n->firm_relation = ir_relation_less; };
void IRBuilder::dispatch(std::shared_ptr<LessThanOrEqual> n) { n->firm_relation = ir_relation_less_equal; };
void IRBuilder::dispatch(std::shared_ptr<GreaterThan> n) { n->firm_relation = ir_relation_greater; };
void IRBuilder::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { n->firm_relation = ir_relation_greater_equal; };





void IRBuilder::dispatch(std::shared_ptr<CNull> n) { };
void IRBuilder::dispatch(std::shared_ptr<CThis> n) { };
void IRBuilder::dispatch(std::shared_ptr<CTrue> n) { };
void IRBuilder::dispatch(std::shared_ptr<CFalse> n) { };
void IRBuilder::dispatch(std::shared_ptr<Add> n) { };
void IRBuilder::dispatch(std::shared_ptr<Subtract> n) { };
void IRBuilder::dispatch(std::shared_ptr<Multiply> n) { };
void IRBuilder::dispatch(std::shared_ptr<Divide> n) { };
void IRBuilder::dispatch(std::shared_ptr<Modulo> n) { };
void IRBuilder::dispatch(std::shared_ptr<Negate> n) { };
void IRBuilder::dispatch(std::shared_ptr<Minus> n) { };
