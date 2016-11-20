#include "generalchecker.h"


using namespace cmpl;


inline void GeneralChecker::error(const std::string &err)
{
  throw MultipleMainMethodsError(err.c_str());
}

std::shared_ptr<Type> GeneralChecker::voidNode() {
  return std::make_shared<Type>(std::make_shared<TypeVoid>(), 0);
}
  
void GeneralChecker::dispatch(std::shared_ptr<Program> n) {
  for(auto const& c: n->classDeclarations) {	  
    c->accept(shared_from_this());
  }
};

void GeneralChecker::dispatch(std::shared_ptr<ClassDeclaration> n) {
  for (auto const& m: n->classMembers) {
    m->accept(shared_from_this()); 
  }
};

void GeneralChecker::dispatch(std::shared_ptr<MainMethod> n) {
	mainMethods++;
	if(mainMethods>1)
	{
		error("Found multiple MainMethod definitions!");
	}
 };
void GeneralChecker::dispatch(std::shared_ptr<Field> n) {  };
void GeneralChecker::dispatch(std::shared_ptr<Method> n) {};
void GeneralChecker::dispatch(std::shared_ptr<Type> n) {};
void GeneralChecker::dispatch(std::shared_ptr<UserType> n) { };
void GeneralChecker::dispatch(std::shared_ptr<TypeInt> n) { };
void GeneralChecker::dispatch(std::shared_ptr<TypeBoolean> n) { };
void GeneralChecker::dispatch(std::shared_ptr<TypeVoid> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Parameter> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Block> n) {};
void GeneralChecker::dispatch(std::shared_ptr<IfStatement> n) { };
void GeneralChecker::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void GeneralChecker::dispatch(std::shared_ptr<WhileStatement> n) { };
void GeneralChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void GeneralChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) { };
void GeneralChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };
void GeneralChecker::dispatch(std::shared_ptr<IfElseStatement> n) {};
void GeneralChecker::dispatch(std::shared_ptr<ReturnStatement> n) {};
void GeneralChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {};
void GeneralChecker::dispatch(std::shared_ptr<MethodInvocation> n) { };
void GeneralChecker::dispatch(std::shared_ptr<ArrayAccess> n) { };
void GeneralChecker::dispatch(std::shared_ptr<FieldAccess> n) { };
void GeneralChecker::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<EqualityExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<RelationalExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CallExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CNull> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CThis> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CTrue> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CFalse> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CRef> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void GeneralChecker::dispatch(std::shared_ptr<NewObject> n) { };
void GeneralChecker::dispatch(std::shared_ptr<NewArray> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Equals> n) { };
void GeneralChecker::dispatch(std::shared_ptr<NotEquals> n) { };
void GeneralChecker::dispatch(std::shared_ptr<LessThan> n) { };
void GeneralChecker::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void GeneralChecker::dispatch(std::shared_ptr<GreaterThan> n) { };
void GeneralChecker::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Add> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Subtract> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Multiply> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Divide> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Modulo> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Negate> n) { };
void GeneralChecker::dispatch(std::shared_ptr<Minus> n) { };
