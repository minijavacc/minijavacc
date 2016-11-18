#include "staticdeclarationscollector.h"

#include <iostream>

using namespace cmpl;


// helpers

class CollectorError : public std::runtime_error
{
public:
  CollectorError(const std::string& err) : std::runtime_error(err) { }
};

inline void error(const std::string &err)
{
  throw CollectorError(err);
}



void StaticDeclarationsCollector::dispatch(std::shared_ptr<Program> n) {
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<ClassDeclaration> n) {
  
  
  currentClassDeclaration = n;
  for (auto const& c : n->classMembers) {
    c->accept(shared_from_this());
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<Field> n) {
  if (currentClassDeclaration->fields.count(n->ID) > 0) {
    error("cannot have multiple fields with the same name");
    return;
  }
  
  currentClassDeclaration->fields.emplace(n->ID, n);
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<Method> n) {
  if (currentClassDeclaration->methods.count(n->ID) > 0) {
    error("cannot have multiple methods with the same name");
    return;
  }
  
  currentClassDeclaration->methods.emplace(n->ID, n);
  
  // collect parameters
  currentMethod = n;
  
  for (auto const& p : n->parameters) {
    p->accept(shared_from_this());
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<Parameter> n) {
  if (currentMethod->parameterMap.count(n->ID) > 0) {
    error("cannot have multiple parameters with the same name");
    return;
  }
  
  currentMethod->parameterMap.emplace(n->ID, n);
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<MainMethod> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Type> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<UserType> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<TypeInt> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<TypeBoolean> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<TypeVoid> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Block> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<IfStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<WhileStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<EmptyStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<IfElseStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<ReturnStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<ReturnExpressionStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<MethodInvocation> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<ArrayAccess> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<FieldAccess> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<EqualityExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<RelationalExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CallExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CNull> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CThis> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CTrue> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CFalse> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CRef> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<NewObject> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<NewArray> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Equals> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<NotEquals> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LessThan> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<GreaterThan> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Add> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Subtract> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Multiply> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Divide> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Modulo> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Negate> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Minus> n) { };
