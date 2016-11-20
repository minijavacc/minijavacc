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
  if(mainMethods==0)
	{
		error("Found no MainMethod definition!");
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
	 n->block->accept(shared_from_this());
 };
 
 void GeneralChecker::dispatch(std::shared_ptr<CThis> n) {
  error("Found \"this\" in MainMethod!");
};

void GeneralChecker::dispatch(std::shared_ptr<Field> n) { };

void GeneralChecker::dispatch(std::shared_ptr<Method> n) {
 //dont look in methods, only mainmethod
};
 
 void GeneralChecker::dispatch(std::shared_ptr<Block> n) {
	  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
  }
};

void GeneralChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  n->type->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->expression->accept(shared_from_this());
  n->type->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<IfStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<WhileStatement> n) {
  n->expression->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };
void GeneralChecker::dispatch(std::shared_ptr<ReturnStatement> n) { };

void GeneralChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<MethodInvocation> n) {
  for (auto const& a : n->arguments) {
    a->accept(shared_from_this());
  }
};

void GeneralChecker::dispatch(std::shared_ptr<ArrayAccess> n) {
  n->expression->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<FieldAccess> n) {};

void GeneralChecker::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<CallExpression> n) {
  for (int i = 0; i < n->arguments.size(); i++) {
    n->arguments[i]->accept(shared_from_this());
  }
};

void GeneralChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<CRef> n) {};

void GeneralChecker::dispatch(std::shared_ptr<NewObject> n) {
  n->userType->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<NewArray> n) {
  n->expression->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<Type> n) {
  n->basicType->accept(shared_from_this());
};

void GeneralChecker::dispatch(std::shared_ptr<UserType> n) {};
void GeneralChecker::dispatch(std::shared_ptr<Parameter> n) {};
void GeneralChecker::dispatch(std::shared_ptr<TypeInt> n) { };
void GeneralChecker::dispatch(std::shared_ptr<TypeBoolean> n) { };
void GeneralChecker::dispatch(std::shared_ptr<TypeVoid> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CNull> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CTrue> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CFalse> n) { };
void GeneralChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
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
