#include "typecreator.h"


using namespace cmpl;


inline void TypeCreator::error(const std::string &err)
{
  throw TypeCreatorError(("TypeCreator: " + err).c_str());
}

void TypeCreator::dispatch(std::shared_ptr<Program> n) {
  ir_type *type = new_type_struct(new_id_from_str("yo"));
  
  // saves types in AST -> only one object per type required?
};

void TypeCreator::dispatch(std::shared_ptr<ClassDeclaration> n) { };
void TypeCreator::dispatch(std::shared_ptr<MainMethod> n) { };
void TypeCreator::dispatch(std::shared_ptr<Field> n) { };
void TypeCreator::dispatch(std::shared_ptr<Method> n) { };
void TypeCreator::dispatch(std::shared_ptr<Type> n) { };
void TypeCreator::dispatch(std::shared_ptr<FakeType> n) { };
void TypeCreator::dispatch(std::shared_ptr<NullType> n) { };
void TypeCreator::dispatch(std::shared_ptr<UserType> n) { };
void TypeCreator::dispatch(std::shared_ptr<TypeInt> n) { };
void TypeCreator::dispatch(std::shared_ptr<TypeBoolean> n) { };
void TypeCreator::dispatch(std::shared_ptr<TypeVoid> n) { };
void TypeCreator::dispatch(std::shared_ptr<Parameter> n) { };
void TypeCreator::dispatch(std::shared_ptr<Block> n) { };
void TypeCreator::dispatch(std::shared_ptr<IfStatement> n) { };
void TypeCreator::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void TypeCreator::dispatch(std::shared_ptr<WhileStatement> n) { };
void TypeCreator::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void TypeCreator::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) { };
void TypeCreator::dispatch(std::shared_ptr<EmptyStatement> n) { };
void TypeCreator::dispatch(std::shared_ptr<IfElseStatement> n) { };
void TypeCreator::dispatch(std::shared_ptr<ReturnStatement> n) { };
void TypeCreator::dispatch(std::shared_ptr<ReturnExpressionStatement> n) { };
void TypeCreator::dispatch(std::shared_ptr<MethodInvocation> n) { };
void TypeCreator::dispatch(std::shared_ptr<ArrayAccess> n) { };
void TypeCreator::dispatch(std::shared_ptr<FieldAccess> n) { };
void TypeCreator::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<EqualityExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<RelationalExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<CallExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<CNull> n) { };
void TypeCreator::dispatch(std::shared_ptr<CThis> n) { };
void TypeCreator::dispatch(std::shared_ptr<CTrue> n) { };
void TypeCreator::dispatch(std::shared_ptr<CFalse> n) { };
void TypeCreator::dispatch(std::shared_ptr<CRef> n) { };
void TypeCreator::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void TypeCreator::dispatch(std::shared_ptr<NewObject> n) { };
void TypeCreator::dispatch(std::shared_ptr<NewArray> n) { };
void TypeCreator::dispatch(std::shared_ptr<StaticLibraryCallExpression> n) { };
void TypeCreator::dispatch(std::shared_ptr<Equals> n) { };
void TypeCreator::dispatch(std::shared_ptr<NotEquals> n) { };
void TypeCreator::dispatch(std::shared_ptr<LessThan> n) { };
void TypeCreator::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void TypeCreator::dispatch(std::shared_ptr<GreaterThan> n) { };
void TypeCreator::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void TypeCreator::dispatch(std::shared_ptr<Add> n) { };
void TypeCreator::dispatch(std::shared_ptr<Subtract> n) { };
void TypeCreator::dispatch(std::shared_ptr<Multiply> n) { };
void TypeCreator::dispatch(std::shared_ptr<Divide> n) { };
void TypeCreator::dispatch(std::shared_ptr<Modulo> n) { };
void TypeCreator::dispatch(std::shared_ptr<Negate> n) { };
void TypeCreator::dispatch(std::shared_ptr<Minus> n) { };
