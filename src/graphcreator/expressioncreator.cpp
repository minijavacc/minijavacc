#include "expressioncreator.h"


using namespace cmpl;


inline void ExpressionCreator::error(const std::string &err)
{
  throw ExpressionCreatorError(("ExpressionCreator: " + err).c_str());
}

void ExpressionCreator::dispatch(std::shared_ptr<Program> n) {
  ir_type *type = new_type_struct(new_id_from_str("yo"));
  
  // saves types in AST -> only one object per type required?
};

void ExpressionCreator::dispatch(std::shared_ptr<ClassDeclaration> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<MainMethod> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Field> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Method> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Type> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<FakeType> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<UserType> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<TypeInt> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<TypeBoolean> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<TypeVoid> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Parameter> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Block> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<IfStatement> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<WhileStatement> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<EmptyStatement> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<IfElseStatement> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<ReturnStatement> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<ReturnExpressionStatement> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<MethodInvocation> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<ArrayAccess> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<FieldAccess> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<EqualityExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<RelationalExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CallExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CNull> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CThis> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CTrue> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CFalse> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CRef> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<NewObject> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<NewArray> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Equals> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<NotEquals> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<LessThan> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<GreaterThan> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Add> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Subtract> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Multiply> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Divide> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Modulo> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Negate> n) { };
void ExpressionCreator::dispatch(std::shared_ptr<Minus> n) { };
