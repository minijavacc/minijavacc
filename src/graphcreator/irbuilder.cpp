#include "irbuilder.h"
#include <assert.h>
#include "../stringtable/stringtable.h"


using namespace cmpl;


inline void IRBuilder::error(const std::string &err)
{
  throw IRBuilderError(("IRBuilder: " + err).c_str());
}

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
  ir_graph *g = n->getFirmGraph();
  set_current_ir_graph(g);
  set_r_cur_block(g, get_irg_start_block(g));
  
  n->block->accept(shared_from_this());
  
  ir_node *ret = new_Return(get_store(), 0, NULL);
  
  ir_node *end = get_irg_end_block(g);
  add_immBlock_pred(end, ret);
  mature_immBlock(get_r_cur_block(g));
  
  irg_finalize_cons(g);
};

void IRBuilder::dispatch(std::shared_ptr<Field> n) { };

void IRBuilder::dispatch(std::shared_ptr<Method> n) {
  ir_graph *g = n->getFirmGraph();
  set_current_ir_graph(g);
  set_r_cur_block(g, get_irg_start_block(g));
  ir_node *args = get_irg_args(g);
  
  int i = 0;
  for (auto const& p : n->parameters) {
    p->firm_node = new_Proj(args, p->type->getFirmMode(), i++);
  }
  
  n->block->accept(shared_from_this());
  
  irg_finalize_cons(g);
//  ir_node *end = get_irg_end_block(g);
//  mature_immBlock(end);
};

void IRBuilder::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s : n->statements) {
    s->accept(shared_from_this());
  }
};

void IRBuilder::dispatch(std::shared_ptr<Type> n) { };
void IRBuilder::dispatch(std::shared_ptr<FakeType> n) { };
void IRBuilder::dispatch(std::shared_ptr<NullType> n) { };
void IRBuilder::dispatch(std::shared_ptr<UserType> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeInt> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeBoolean> n) { };
void IRBuilder::dispatch(std::shared_ptr<TypeVoid> n) { };
void IRBuilder::dispatch(std::shared_ptr<Parameter> n) { };

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

void IRBuilder::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {

};

void IRBuilder::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->expression->accept(shared_from_this());
  n->setDefinition(n->expression->firm_node);
};

void IRBuilder::dispatch(std::shared_ptr<EmptyStatement> n) { };

void IRBuilder::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
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

void IRBuilder::dispatch(std::shared_ptr<MethodInvocation> n) { };
void IRBuilder::dispatch(std::shared_ptr<ArrayAccess> n) { };
void IRBuilder::dispatch(std::shared_ptr<FieldAccess> n) { };
void IRBuilder::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void IRBuilder::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void IRBuilder::dispatch(std::shared_ptr<CallExpression> n) { };

void IRBuilder::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression2->accept(shared_from_this());
  n->expression1->assign(n->expression2->firm_node);
  n->firm_node = n->expression2->firm_node;
};

void IRBuilder::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  n->op->accept(shared_from_this());
  n->firm_node = new_Cmp(n->expression1->firm_node, n->expression2->firm_node, n->op->firm_relation);
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

void IRBuilder::dispatch(std::shared_ptr<CIntegerLiteral> n) {
  ir_tarval *tv = new_tarval_from_long(n->value, mode_Is);
  n->firm_node = new_Const(tv);
};

void IRBuilder::dispatch(std::shared_ptr<CRef> n) {
  auto d = n->declaration.lock();
  assert(d);
  
  n->firm_node = d->firm_node;
};

void IRBuilder::dispatch(std::shared_ptr<Equals> n) { n->firm_relation = ir_relation_equal; };
void IRBuilder::dispatch(std::shared_ptr<NotEquals> n) { n->firm_relation = ir_relation_less_greater; };
void IRBuilder::dispatch(std::shared_ptr<LessThan> n) { n->firm_relation = ir_relation_less; };
void IRBuilder::dispatch(std::shared_ptr<LessThanOrEqual> n) { n->firm_relation = ir_relation_less_equal; };
void IRBuilder::dispatch(std::shared_ptr<GreaterThan> n) { n->firm_relation = ir_relation_greater; };
void IRBuilder::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { n->firm_relation = ir_relation_greater_equal; };

void IRBuilder::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void IRBuilder::dispatch(std::shared_ptr<CNull> n) { };
void IRBuilder::dispatch(std::shared_ptr<CThis> n) { };
void IRBuilder::dispatch(std::shared_ptr<CTrue> n) { };
void IRBuilder::dispatch(std::shared_ptr<CFalse> n) { };
void IRBuilder::dispatch(std::shared_ptr<NewObject> n) { };
void IRBuilder::dispatch(std::shared_ptr<NewArray> n) { };
void IRBuilder::dispatch(std::shared_ptr<StaticLibraryCallExpression> n) { };
void IRBuilder::dispatch(std::shared_ptr<Add> n) { };
void IRBuilder::dispatch(std::shared_ptr<Subtract> n) { };
void IRBuilder::dispatch(std::shared_ptr<Multiply> n) { };
void IRBuilder::dispatch(std::shared_ptr<Divide> n) { };
void IRBuilder::dispatch(std::shared_ptr<Modulo> n) { };
void IRBuilder::dispatch(std::shared_ptr<Negate> n) { };
void IRBuilder::dispatch(std::shared_ptr<Minus> n) { };
