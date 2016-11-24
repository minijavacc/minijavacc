#include "voidtypechecker.h"
#include "singleton.h"

using namespace cmpl;

/***** helper functions *****/
inline void VoidTypeChecker::error(const std::string &err) {
  throw VoidTypeError(("voidtypechecker: " + err).c_str());
}

inline void VoidTypeChecker::error(const std::string &err, const std::shared_ptr<Node> &n)
{
  throw VoidTypeError(("voidtypechecker: " + err + ": " + Checker::printNode(n)).c_str());
}

inline void VoidTypeChecker::assureNotVoid(const std::shared_ptr<Type> &type) {
  if(type->basicType->equals(voidBasicNode)) {
    error("Only methods can be of type void.");
  }
}
/****************************/


void VoidTypeChecker::dispatch(std::shared_ptr<Program> n) {
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void VoidTypeChecker::dispatch(std::shared_ptr<ClassDeclaration> n) {
  for (auto const& m: n->classMembers) {
    m->accept(shared_from_this());
  }
};

void VoidTypeChecker::dispatch(std::shared_ptr<MainMethod> n) {
  n->block->accept(shared_from_this());
};

void VoidTypeChecker::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
  }
};

void VoidTypeChecker::dispatch(std::shared_ptr<Method> n) {
  // only place where void is allowed is as method type
  // assure no weird things like void[]
  if(n->type->basicType->equals(voidBasicNode) && n->type->arrayDepth>0) {
    error("A void array? Seriously? Not even telling you where it is.");
  }
  
  for (auto const& p: n->parameters) {
    p->accept(shared_from_this());
  }
  n->block->accept(shared_from_this());
};

void VoidTypeChecker::dispatch(std::shared_ptr<Field> n) {
  assureNotVoid(n->type);
};

void VoidTypeChecker::dispatch(std::shared_ptr<Parameter> n) {
  assureNotVoid(n->type);
};

void VoidTypeChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  assureNotVoid(n->type);
  n->expression->accept(shared_from_this());
};

void VoidTypeChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  assureNotVoid(n->type);
};

void VoidTypeChecker::dispatch(std::shared_ptr<IfStatement> n) {
  n->ifStatement->accept(shared_from_this());
};

void VoidTypeChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
};

void VoidTypeChecker::dispatch(std::shared_ptr<ExpressionStatement> n) { };

void VoidTypeChecker::dispatch(std::shared_ptr<WhileStatement> n) {
  n->statement->accept(shared_from_this());
};

void VoidTypeChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<MethodInvocation> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<ArrayAccess> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<EqualityExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<RelationalExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<CallExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<NewArray> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<ReturnStatement> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<FieldAccess> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<CRef> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<NewObject> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Type> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<FakeType> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<UserType> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<TypeInt> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<TypeBoolean> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<TypeVoid> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<CNull> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<CThis> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<CTrue> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<CFalse> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Equals> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<NotEquals> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<LessThan> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<GreaterThan> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Add> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Subtract> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Multiply> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Divide> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Modulo> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Negate> n) { };
void VoidTypeChecker::dispatch(std::shared_ptr<Minus> n) { };
