#include "returnchecker.h"


using namespace cmpl;


inline void ReturnChecker::error(const std::string &err)
{
  throw MissingReturnPathError(("returnchecker: " + err).c_str());
}

void ReturnChecker::dispatch(std::shared_ptr<Program> n) {
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void ReturnChecker::dispatch(std::shared_ptr<ClassDeclaration> n) {
  for (auto const& m: n->classMembers) {
    m->accept(shared_from_this());
  }
};

void ReturnChecker::dispatch(std::shared_ptr<MainMethod> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Field> n) { };

/* Checks whether
 a) method has type void and no return statement
 b) method has other type and the method returns on all paths */
void ReturnChecker::dispatch(std::shared_ptr<Method> n) {
  currentMethodIsVoid = n->type->equals(voidNode);
  
  n->block->accept(shared_from_this());
  
  if (n->block->returns) { // did return
    if(currentMethodIsVoid) {
      error("Method " + StringTable::lookupIdentifier(n->ID) + " of type void would return a value");
    }
  } else {
    if(!currentMethodIsVoid) {
      error("Method " + StringTable::lookupIdentifier(n->ID) + " has missing return paths");
    }
  }
};

void ReturnChecker::dispatch(std::shared_ptr<Type> n) { };
void ReturnChecker::dispatch(std::shared_ptr<FakeType> n) { };
void ReturnChecker::dispatch(std::shared_ptr<UserType> n) { };
void ReturnChecker::dispatch(std::shared_ptr<TypeInt> n) { };
void ReturnChecker::dispatch(std::shared_ptr<TypeBoolean> n) { };
void ReturnChecker::dispatch(std::shared_ptr<TypeVoid> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Parameter> n) { };

// Returns if one of the statements returns
void ReturnChecker::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
    
    // One of the statements has to return
    if (s->returns) {
      n->returns = true;
      break;
    }
  } // dead code recognition could be done here
};

// Do not return
void ReturnChecker::dispatch(std::shared_ptr<IfStatement> n) { };
void ReturnChecker::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void ReturnChecker::dispatch(std::shared_ptr<WhileStatement> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) { };
void ReturnChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };

// Returns if both paths return
void ReturnChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
  
  n->returns = n->ifStatement->returns && n->elseStatement->returns;
};

// Always returns
void ReturnChecker::dispatch(std::shared_ptr<ReturnStatement> n) {
  if (!currentMethodIsVoid) { // blank return statement is only allowed in void methods
    error("blank return statement in non-void method");
  }
};

void ReturnChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->returns = true;
};

// Unreachable
void ReturnChecker::dispatch(std::shared_ptr<MethodInvocation> n) { };
void ReturnChecker::dispatch(std::shared_ptr<ArrayAccess> n) { };
void ReturnChecker::dispatch(std::shared_ptr<FieldAccess> n) { };
void ReturnChecker::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<EqualityExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<RelationalExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CallExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CNull> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CThis> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CTrue> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CFalse> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CRef> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void ReturnChecker::dispatch(std::shared_ptr<NewObject> n) { };
void ReturnChecker::dispatch(std::shared_ptr<NewArray> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Equals> n) { };
void ReturnChecker::dispatch(std::shared_ptr<NotEquals> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LessThan> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void ReturnChecker::dispatch(std::shared_ptr<GreaterThan> n) { };
void ReturnChecker::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Add> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Subtract> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Multiply> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Divide> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Modulo> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Negate> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Minus> n) { };
