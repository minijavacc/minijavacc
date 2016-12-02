#include "irbuilder.h"
#include <assert.h>
#include "../stringtable/stringtable.h"
#include <libfirm/firm.h>
#include <iostream>
#include "types.h"


using namespace cmpl;




#pragma mark - Helpers

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
    calloc_ent = new_entity(get_glob_type(), new_id_from_str("calloc"), calloc_type);
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





#pragma mark -

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
  
  n->block->accept(shared_from_this());
  
  ir_node *ret = new_Return(get_store(), 0, NULL);
  
  ir_node *end = get_irg_end_block(g);
  add_immBlock_pred(end, ret);
  mature_immBlock(get_r_cur_block(g));
  
  irg_finalize_cons(g);
};

void IRBuilder::dispatch(std::shared_ptr<Field> n) {
};

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
  }
};

void IRBuilder::dispatch(std::shared_ptr<IfStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
};

void IRBuilder::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void IRBuilder::dispatch(std::shared_ptr<WhileStatement> n) {
  n->expression->accept(shared_from_this());
  n->statement->accept(shared_from_this());
};

void IRBuilder::dispatch(std::shared_ptr<EmptyStatement> n) { };

void IRBuilder::dispatch(std::shared_ptr<IfElseStatement> n) {
  ir_graph *g = get_current_ir_graph();
  
  trueBlock = new_r_immBlock(g);
  falseBlock = new_r_immBlock(g);
  nextBlock = new_r_immBlock(g);
  
  n->expression->accept(shared_from_this());
  
  set_cur_block(trueBlock);
  n->ifStatement->accept(shared_from_this());
  ir_node *jmpIf = new_Jmp();
  
  set_cur_block(falseBlock);
  n->elseStatement->accept(shared_from_this());
  ir_node *jmpElse = new_Jmp();
  
  add_immBlock_pred(nextBlock, jmpIf);
  mature_immBlock(trueBlock);
  add_immBlock_pred(nextBlock, jmpElse);
  mature_immBlock(falseBlock);
  
  set_cur_block(nextBlock);

};

void IRBuilder::dispatch(std::shared_ptr<ReturnStatement> n) {
  n->firm_node = new_Return(get_store(), 0, NULL);
  
  ir_graph *g = get_current_ir_graph();
  ir_node *end = get_irg_end_block(g);
  add_immBlock_pred(end, n->firm_node);
  mature_immBlock(get_r_cur_block(g));
};

void IRBuilder::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->expression->accept(shared_from_this());
  ir_node *results[1] = { n->expression->firm_node };
  n->firm_node = new_Return(get_store(), 1, results);
  
  ir_graph *g = get_current_ir_graph();
  ir_node *end = get_irg_end_block(g);
  add_immBlock_pred(end, n->firm_node);
  mature_immBlock(get_r_cur_block(g));
};




#pragma mark - Types

void IRBuilder::dispatch(std::shared_ptr<Type> n) { };
void IRBuilder::dispatch(std::shared_ptr<FakeType> n) { };
void IRBuilder::dispatch(std::shared_ptr<NullType> n) { };
void IRBuilder::dispatch(std::shared_ptr<UserType> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeInt> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeBoolean> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeVoid> n) { };




#pragma mark - Local variables

void IRBuilder::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {

};

void IRBuilder::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->expression->accept(shared_from_this());
  n->firm_node = n->expression->firm_node;
};





#pragma mark - Expressions

void IRBuilder::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression2->accept(shared_from_this());
  
  if (auto lhs = dynamic_cast<CRef*>(n->expression1.get())) {
    auto d = lhs->declaration.lock();
    
    if (auto f = dynamic_cast<Field*>(d.get())) {
      // is field access
      ir_graph *g = get_current_ir_graph();
      ir_node *args = get_irg_args(g);
      ir_node *this_node = new_Proj(args, mode_P, 0);
      
      ir_node *irn = new_Member(this_node, f->firm_entity);
      ir_node *st  = new_Store(get_store(), irn, n->expression2->firm_node, f->type->getFirmType(), cons_none);
      ir_node *m   = new_Proj(st, mode_M, pn_Store_M);
      set_store(m);
    } else {
      // is local var access
      set_value(d->parameterIndex, n->expression2->firm_node);
    }
  }
  
  if (auto ure = dynamic_cast<UnaryRightExpression*>(n->expression1.get())) {
    ure->expression->accept(shared_from_this());
    
    if (auto fa = dynamic_cast<FieldAccess*>(ure->op.get())) {
      auto decl = fa->declaration.lock();
      ir_node *irn = new_Member(ure->expression->firm_node, decl->firm_entity);
      ir_node *st  = new_Store(get_store(), irn, n->expression2->firm_node, decl->type->getFirmType(), cons_none);
      ir_node *m   = new_Proj(st, mode_M, pn_Store_M);
      set_store(m);
    }
    
    if (auto aa = dynamic_cast<ArrayAccess*>(ure->op.get())) {
      // currentExpression->firm_node is a Proj P to an array_type
      aa->expression->accept(shared_from_this());
      
      ir_type *array_type = get_pointer_points_to_type(ure->expression->type->getFirmType());
      ir_mode *array_mode = get_type_mode(array_type);
      ir_type *elem_type  = get_array_element_type(array_type);
      ir_mode *elem_mode  = get_type_mode(elem_type);
      
      ir_node *sel       = new_Sel(ure->expression->firm_node, aa->expression->firm_node, array_type);
      ir_node *st        = new_Store(get_store(), sel, n->expression2->firm_node, elem_type, cons_none);
      ir_node *m         = new_Proj(st, mode_M, pn_Load_M);
      
      set_store(m);
    }
  }
  
  n->firm_node = n->expression2->firm_node;
};

void IRBuilder::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void IRBuilder::dispatch(std::shared_ptr<LogicalAndExpression> n) { };

void IRBuilder::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  n->op->accept(shared_from_this());

  ir_node *cmp = new_Cmp(n->expression1->firm_node, n->expression2->firm_node, n->op->firm_relation);
  ir_node *cond = new_Cond(cmp);
  ir_node *tnode = new_Proj(cond, mode_X, pn_Cond_true);
  ir_node *fnode = new_Proj(cond, mode_X, pn_Cond_false);
  
  add_immBlock_pred(trueBlock, tnode);
  add_immBlock_pred(falseBlock, fnode);
};

void IRBuilder::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  n->op->accept(shared_from_this());
  n->firm_node = new_Cmp(n->expression1->firm_node, n->expression2->firm_node, n->op->firm_relation);
};

void IRBuilder::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (dynamic_cast<Add*>(n->op.get())) {
    n->firm_node = new_Add(n->expression1->firm_node, n->expression2->firm_node);
    return;
  }
  
  if (dynamic_cast<Subtract*>(n->op.get())) {
    n->firm_node = new_Sub(n->expression1->firm_node, n->expression2->firm_node);
    return;
  }
  
  assert(false);
};

void IRBuilder::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (dynamic_cast<Multiply*>(n->op.get())) {
    n->firm_node = new_Mul(n->expression1->firm_node, n->expression2->firm_node);
    return;
  }
  
  if (dynamic_cast<Divide*>(n->op.get())) {
    ir_node *mem = get_store();
    ir_node* div = new_Div(mem, n->expression1->firm_node, n->expression2->firm_node, 0); // TODO: what is pin?
    n->firm_node = new_Proj(div, mode_Is, pn_Div_res);
    return;
  }
  
  if (dynamic_cast<Modulo*>(n->op.get())) {
    ir_node *mem = get_store();
    ir_node* div = new_Mod(mem, n->expression1->firm_node, n->expression2->firm_node, 0); // TODO: what is pin?
    n->firm_node = new_Proj(div, mode_Is, pn_Mod_res);
    mem = new_Proj(div, mode_M, pn_Mod_M);
    set_store(mem);
    return;
  }
  
  assert(false);
};

void IRBuilder::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
  
  if (dynamic_cast<Minus*>(n->op.get())) {
    n->firm_node = new_Minus(n->expression->firm_node);
    return;
  }
};

void IRBuilder::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
  currentExpression = n->expression;
  n->op->accept(shared_from_this());
  currentExpression = nullptr;
  
  n->firm_node = n->op->firm_node;
};

void IRBuilder::dispatch(std::shared_ptr<FieldAccess> n) {
  auto decl = n->declaration.lock();
  
  // currentExpression->firm_node is a Proj P64 to a class entity
  ir_node *irn = new_Member(currentExpression->firm_node, decl->firm_entity);
  ir_mode *mode = get_type_mode(decl->type->getFirmType());
  ir_node *ld  = new_Load(get_store(), irn, mode, decl->type->getFirmType(), cons_none);
  ir_node *m   = new_Proj(ld, mode_M, pn_Load_M);
  ir_node *res = new_Proj(ld, mode, pn_Load_res);
  
  set_store(m);
  n->firm_node = res;
};

void IRBuilder::dispatch(std::shared_ptr<MethodInvocation> n) { };

void IRBuilder::dispatch(std::shared_ptr<ArrayAccess> n) {
  // currentExpression->firm_node is a Proj P to an array_type
  n->expression->accept(shared_from_this());
  
  ir_type *array_type = get_pointer_points_to_type(currentExpression->type->getFirmType());
  ir_mode *array_mode = get_type_mode(array_type);
  ir_type *elem_type  = get_array_element_type(array_type);
  ir_mode *elem_mode  = get_type_mode(elem_type);
  
  ir_node *sel       = new_Sel(currentExpression->firm_node, n->expression->firm_node, array_type);
  ir_node *ld        = new_Load(get_store(), sel, elem_mode, elem_type, cons_none);
  ir_node *m         = new_Proj(ld, mode_M, pn_Load_M);
  ir_node *res       = new_Proj(ld, elem_mode, pn_Load_res);
  
  set_store(m);
  n->firm_node = res;
};






void IRBuilder::dispatch(std::shared_ptr<CallExpression> n) {
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
    a->accept(shared_from_this());
    args[i++] = a->firm_node;
  }
  
  ir_node *call = new_Call(get_store(), addr, (int)nargs + 1, args, decl->type->getFirmType());
  ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
  ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
  ir_mode *mode = get_type_mode(decl->type->getFirmType());
  ir_node *res = new_Proj(tres, mode, 0);
  
  set_store(mem);
  n->firm_node = res;
};

void IRBuilder::dispatch(std::shared_ptr<NewObject> n) {
  ir_node *num = new_Const(new_tarval_from_long(1, mode_Is));
  ir_node *call = callCallocNode(num, n->type->getFirmType());
  ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
  ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
  ir_node *res = new_Proj(tres, mode_P, 0);
  
  set_store(mem);
  n->firm_node = res;
};

void IRBuilder::dispatch(std::shared_ptr<NewArray> n) {
  n->expression->accept(shared_from_this());
  
  ir_node *num = n->expression->firm_node;
  ir_node *call = callCallocNode(num, n->type->getFirmType());
  ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
  ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
  ir_node *res = new_Proj(tres, mode_P, 0);
  
  set_store(mem);
  n->firm_node = res;
};







void IRBuilder::dispatch(std::shared_ptr<CIntegerLiteral> n) {
  ir_tarval *tv = new_tarval_from_long(n->value, mode_Is);
  n->firm_node = new_Const(tv);
};

void IRBuilder::dispatch(std::shared_ptr<CRef> n) {
  auto d = n->declaration.lock();
  assert(d);
  
  if (auto decl = dynamic_cast<Field*>(d.get())) {
    // is field access
    ir_graph *g = get_current_ir_graph();
    ir_node *args = get_irg_args(g);
    ir_node *this_node = new_Proj(args, mode_P, 0);
    
    ir_node *irn = new_Member(this_node, decl->firm_entity);
    ir_mode *mode = get_type_mode(decl->type->getFirmType());
    ir_node *ld  = new_Load(get_store(), irn, mode, decl->type->getFirmType(), cons_none);
    ir_node *m   = new_Proj(ld, mode_M, pn_Load_M);
    ir_node *res = new_Proj(ld, mode, pn_Load_res);
    
    set_store(m);
    n->firm_node = res;
  } else {
    // is local var access
    if (auto decl = dynamic_cast<TypedNode*>(d.get())) {
      n->firm_node = get_value(d->parameterIndex, get_type_mode(decl->type->getFirmType()));
    } else {
      assert(false);
    }
  }
};

void IRBuilder::dispatch(std::shared_ptr<StaticLibraryCallExpression> n) {
  ir_entity *meth = n->getFirmEntity();
  
  // println() only has one argument (no this-pointer!)
  ir_node *addr = new_Address(meth);
  ir_node *args[1];
  
  n->expression->accept(shared_from_this());
  args[0] = n->expression->firm_node;
  
  ir_node *call = new_Call(get_store(), addr, 1, args, n->getFirmType());
  ir_node *mem = new_Proj(call, mode_M, pn_Call_M);
  ir_node *tres = new_Proj(call, mode_T, pn_Call_T_result);
  //ir_node *res = new_Proj(tres, Types::->getFirmMode(), 0);
  
  set_store(mem);
  
  // returntype is void... what do do?
  n->firm_node = nullptr;
};



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
