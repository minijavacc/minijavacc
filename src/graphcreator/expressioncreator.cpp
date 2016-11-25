#include "expressioncreator.h"
#include <assert.h>
#include "../stringtable/stringtable.h"


using namespace cmpl;


inline void ExpressionCreator::error(const std::string &err)
{
  throw ExpressionCreatorError(("ExpressionCreator: " + err).c_str());
}

void ExpressionCreator::dispatch(std::shared_ptr<Program> n) {
  for (auto const& c : n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void ExpressionCreator::dispatch(std::shared_ptr<ClassDeclaration> n) {
  n->declared_type = new_type_class(new_id_from_str(StringTable::lookupIdentifier(n->ID).c_str()));
  
  for (auto const& c : n->classMembers) {
    c->accept(shared_from_this());
  }
};

void ExpressionCreator::dispatch(std::shared_ptr<MainMethod> n) { };

void ExpressionCreator::dispatch(std::shared_ptr<Field> n) { };

void ExpressionCreator::dispatch(std::shared_ptr<Method> n) {
  ir_type *t = new_type_method(n->parameters.size(), 1, false, cc_cdecl_set, mtp_no_property);
  
  int i = 0;
  for (auto const& p : n->parameters) {
    set_method_param_type(t, i++, p->type->firm_type);
  }
  
  set_method_res_type(t, 0, n->type->firm_type);
  
  n->block->accept(shared_from_this());
};

void ExpressionCreator::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s : n->statements) {
    s->accept(shared_from_this());
  }
};

void ExpressionCreator::dispatch(std::shared_ptr<Type> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<FakeType> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<NullType> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<UserType> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<TypeInt> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<TypeBoolean> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<TypeVoid> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Parameter> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<IfStatement> n) { };

void ExpressionCreator::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void ExpressionCreator::dispatch(std::shared_ptr<WhileStatement> n) { };

void ExpressionCreator::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {

};

void ExpressionCreator::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->expression->accept(shared_from_this());
  n->assign(n->expression->firm_node);
};

void ExpressionCreator::dispatch(std::shared_ptr<EmptyStatement> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<IfElseStatement> n) { };

void ExpressionCreator::dispatch(std::shared_ptr<ReturnStatement> n) {
  n->firm_node = new_Return(get_store(), 0, NULL);
};

void ExpressionCreator::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->expression->accept(shared_from_this());
  ir_node *results[1] = { n->expression->firm_node };
  n->firm_node = new_Return(get_store(), 0, results);
};

void ExpressionCreator::dispatch(std::shared_ptr<MethodInvocation> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<ArrayAccess> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<FieldAccess> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CallExpression> n) { };

void ExpressionCreator::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression2->accept(shared_from_this());
  n->expression1->assign(n->expression2->firm_node);
  n->firm_node = n->expression2->firm_node;
};

void ExpressionCreator::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  n->op->accept(shared_from_this());
  n->firm_node = new_Cmp(n->expression1->firm_node, n->expression2->firm_node, n->op->ir_relation);
};

void ExpressionCreator::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  n->op->accept(shared_from_this());
  n->firm_node = new_Cmp(n->expression1->firm_node, n->expression2->firm_node, n->op->ir_relation);
};

void ExpressionCreator::dispatch(std::shared_ptr<AdditiveExpression> n) {
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

void ExpressionCreator::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
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
    return;
  }
  
  assert(false);
};

void ExpressionCreator::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
  
  if (dynamic_cast<Minus*>(n->op.get())) {
    n->firm_node = new_Minus(n->expression->firm_node);
    return;
  }
};

void ExpressionCreator::dispatch(std::shared_ptr<CIntegerLiteral> n) {
  ir_tarval *tv = new_tarval_from_long(n->value, mode_Is);
  n->firm_node = new_Const(tv);
};

void ExpressionCreator::dispatch(std::shared_ptr<CRef> n) {
  auto d = n->declaration.lock();
  assert(d);
  
  n->firm_node = d->firm_node;
};

void ExpressionCreator::dispatch(std::shared_ptr<Equals> n) { n->ir_relation = ir_relation_equal; };
void ExpressionCreator::dispatch(std::shared_ptr<NotEquals> n) { n->ir_relation = ir_relation_less_greater; };
void ExpressionCreator::dispatch(std::shared_ptr<LessThan> n) { n->ir_relation = ir_relation_less; };
void ExpressionCreator::dispatch(std::shared_ptr<LessThanOrEqual> n) { n->ir_relation = ir_relation_less_equal; };
void ExpressionCreator::dispatch(std::shared_ptr<GreaterThan> n) { n->ir_relation = ir_relation_greater; };
void ExpressionCreator::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { n->ir_relation = ir_relation_greater_equal; };

void ExpressionCreator::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CNull> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CThis> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CTrue> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CFalse> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<NewObject> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<NewArray> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Add> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Subtract> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Multiply> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Divide> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Modulo> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Negate> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Minus> n) { };
