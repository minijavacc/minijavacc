#include "methodcreator.h"


using namespace cmpl;


inline void MethodCreator::error(const std::string &err)
{
  throw MethodCreatorError(("MethodCreator: " + err).c_str());
}

void MethodCreator::dispatch(std::shared_ptr<Program> n) {
  //ir_graph *fun_graph = new_ir_graph(ent, 1); // create a new graph
  //set_current_ir_graph(fun_graph);
  
  // ...
  
  //irg_finalize_cons(fun_graph);
};

void MethodCreator::dispatch(std::shared_ptr<ClassDeclaration> n) { };
void MethodCreator::dispatch(std::shared_ptr<MainMethod> n) { };
void MethodCreator::dispatch(std::shared_ptr<Field> n) { };
void MethodCreator::dispatch(std::shared_ptr<Method> n) { };
void MethodCreator::dispatch(std::shared_ptr<Type> n) { };
void MethodCreator::dispatch(std::shared_ptr<FakeType> n) { };
void MethodCreator::dispatch(std::shared_ptr<UserType> n) { };
void MethodCreator::dispatch(std::shared_ptr<TypeInt> n) { };
void MethodCreator::dispatch(std::shared_ptr<TypeBoolean> n) { };
void MethodCreator::dispatch(std::shared_ptr<TypeVoid> n) { };
void MethodCreator::dispatch(std::shared_ptr<Parameter> n) { };
void MethodCreator::dispatch(std::shared_ptr<Block> n) { };
void MethodCreator::dispatch(std::shared_ptr<IfStatement> n) { };
void MethodCreator::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void MethodCreator::dispatch(std::shared_ptr<WhileStatement> n) { };
void MethodCreator::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void MethodCreator::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) { };
void MethodCreator::dispatch(std::shared_ptr<EmptyStatement> n) { };
void MethodCreator::dispatch(std::shared_ptr<IfElseStatement> n) { };
void MethodCreator::dispatch(std::shared_ptr<ReturnStatement> n) { };
void MethodCreator::dispatch(std::shared_ptr<ReturnExpressionStatement> n) { };
void MethodCreator::dispatch(std::shared_ptr<MethodInvocation> n) { };
void MethodCreator::dispatch(std::shared_ptr<ArrayAccess> n) { };
void MethodCreator::dispatch(std::shared_ptr<FieldAccess> n) { };
void MethodCreator::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<EqualityExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<RelationalExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<CallExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void MethodCreator::dispatch(std::shared_ptr<CNull> n) { };
void MethodCreator::dispatch(std::shared_ptr<CThis> n) { };
void MethodCreator::dispatch(std::shared_ptr<CTrue> n) { };
void MethodCreator::dispatch(std::shared_ptr<CFalse> n) { };
void MethodCreator::dispatch(std::shared_ptr<CRef> n) { };
void MethodCreator::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void MethodCreator::dispatch(std::shared_ptr<NewObject> n) { };
void MethodCreator::dispatch(std::shared_ptr<NewArray> n) { };
void MethodCreator::dispatch(std::shared_ptr<Equals> n) { };
void MethodCreator::dispatch(std::shared_ptr<NotEquals> n) { };
void MethodCreator::dispatch(std::shared_ptr<LessThan> n) { };
void MethodCreator::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void MethodCreator::dispatch(std::shared_ptr<GreaterThan> n) { };
void MethodCreator::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void MethodCreator::dispatch(std::shared_ptr<Add> n) { };
void MethodCreator::dispatch(std::shared_ptr<Subtract> n) { };
void MethodCreator::dispatch(std::shared_ptr<Multiply> n) { };
void MethodCreator::dispatch(std::shared_ptr<Divide> n) { };
void MethodCreator::dispatch(std::shared_ptr<Modulo> n) { };
void MethodCreator::dispatch(std::shared_ptr<Negate> n) { };
void MethodCreator::dispatch(std::shared_ptr<Minus> n) { };
