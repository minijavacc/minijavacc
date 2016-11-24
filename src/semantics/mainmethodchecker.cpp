#include "mainmethodchecker.h"

using namespace cmpl;

inline void MainMethodChecker::error(const std::string &err)
{
  throw MainMethodError(("mainmethodchecker: " + err).c_str());
}

void MainMethodChecker::dispatch(std::shared_ptr<Program> n) {
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
  }
  if (mainMethods==0) {
    error("No main method definition found!");
  }
};

void MainMethodChecker::dispatch(std::shared_ptr<ClassDeclaration> n) {
  for (auto const& m: n->classMembers) {
    m->accept(shared_from_this());
  }
};

void MainMethodChecker::dispatch(std::shared_ptr<MainMethod> n) {
  currentMainMethod = n;
  if(StringTable::lookupIdentifier(n->ID).compare("main")) {
    error("static method needs to be named \"main\"!");
  }
  mainMethods++;
  if(mainMethods > 1)  {
    error("multiple main method definitions found!");
  }
  n->block->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
  }
};

void MainMethodChecker::dispatch(std::shared_ptr<CRef> n) {
  if(n->ID == currentMainMethod->parameterID) {
    error("Parameter of main method must not be used!");
  } else {
    auto shared = n->declaration.lock();
    if(dynamic_cast<Field*>(shared.get())) {
      error("Can not reference field from static context!");
    }
  }
};

void MainMethodChecker::dispatch(std::shared_ptr<CThis> n) {
  error("Can not reference \"this\" in static context!");
};


void MainMethodChecker::dispatch(std::shared_ptr<Method> n) {
  //dont look in other methods, only main method
};


void MainMethodChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->expression->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<IfStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<WhileStatement> n) {
  n->expression->accept(shared_from_this());
  n->statement->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  error("Can not return expression in main method!");
};

void MainMethodChecker::dispatch(std::shared_ptr<MethodInvocation> n) {
  for (auto const& a : n->arguments) {
    a->accept(shared_from_this());
  }
};

void MainMethodChecker::dispatch(std::shared_ptr<ArrayAccess> n) {
  n->expression->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<CallExpression> n) {
  error("can not call method from static context");
};

void MainMethodChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
};


void MainMethodChecker::dispatch(std::shared_ptr<NewArray> n) {
  n->expression->accept(shared_from_this());
};

void MainMethodChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<ReturnStatement> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<FieldAccess> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<NewObject> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Type> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<UserType> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Field> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Parameter> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<TypeInt> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<TypeBoolean> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<TypeVoid> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<CNull> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<CTrue> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<CFalse> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Equals> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<NotEquals> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<LessThan> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<GreaterThan> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Add> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Subtract> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Multiply> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Divide> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Modulo> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Negate> n) { };
void MainMethodChecker::dispatch(std::shared_ptr<Minus> n) { };
