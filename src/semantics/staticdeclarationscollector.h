/*
 * Static Declarations Collector
 * - collects all static declarations and stores them in 'methods', 'fields' and 'parameterMap' in the ClassDeclaration-Node
 * - collects all local variables inside methods and stores them in 'localVariables' in the Method node
 * - checks for multiple methods/fields with the same name (no overloading allowed)
 */

#pragma once

#include "../parser/ast.h"
#include "checker.h"

using namespace std;

namespace cmpl {
  class StaticDeclarationsCollector : public Dispatcher, public std::enable_shared_from_this<StaticDeclarationsCollector> {
    private:
      shared_ptr<ClassDeclaration> currentClassDeclaration;
      shared_ptr<Method> currentMethod;
      std::map<StringIdentifier, std::weak_ptr<ClassDeclaration>> classes;
      shared_ptr<MainMethod> mainMethod;
      void errorMultipleNames(const std::string &err, StringIdentifier ID);

    public:
      void dispatch(std::shared_ptr<Type> n);
      void dispatch(std::shared_ptr<FakeType> n);
      void dispatch(std::shared_ptr<NullType> n);
      void dispatch(std::shared_ptr<UserType> n);
      void dispatch(std::shared_ptr<TypeInt> n);
      void dispatch(std::shared_ptr<TypeBoolean> n);
      void dispatch(std::shared_ptr<TypeVoid> n);
      void dispatch(std::shared_ptr<Program> n);
      void dispatch(std::shared_ptr<ClassDeclaration> n);
      void dispatch(std::shared_ptr<Field> n);
      void dispatch(std::shared_ptr<Method> n);
      void dispatch(std::shared_ptr<MainMethod> n);
      void dispatch(std::shared_ptr<Parameter> n);
      void dispatch(std::shared_ptr<Block> n);
      void dispatch(std::shared_ptr<IfStatement> n);
      void dispatch(std::shared_ptr<IfElseStatement> n);
      void dispatch(std::shared_ptr<ExpressionStatement> n);
      void dispatch(std::shared_ptr<WhileStatement> n);
      void dispatch(std::shared_ptr<LocalVariableDeclaration> n);
      void dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n);
      void dispatch(std::shared_ptr<ReturnStatement> n);
      void dispatch(std::shared_ptr<ReturnExpressionStatement> n);
      void dispatch(std::shared_ptr<EmptyStatement> n);
      void dispatch(std::shared_ptr<MethodInvocation> n);
      void dispatch(std::shared_ptr<ArrayAccess> n);
      void dispatch(std::shared_ptr<FieldAccess> n);
      void dispatch(std::shared_ptr<AssignmentExpression> n);
      void dispatch(std::shared_ptr<LogicalOrExpression> n);
      void dispatch(std::shared_ptr<LogicalAndExpression> n);
      void dispatch(std::shared_ptr<EqualityExpression> n);
      void dispatch(std::shared_ptr<RelationalExpression> n);
      void dispatch(std::shared_ptr<AdditiveExpression> n);
      void dispatch(std::shared_ptr<MultiplicativeExpression> n);
      void dispatch(std::shared_ptr<CallExpression> n);
      void dispatch(std::shared_ptr<UnaryLeftExpression> n);
      void dispatch(std::shared_ptr<UnaryRightExpression> n);
      void dispatch(std::shared_ptr<CNull> n);
      void dispatch(std::shared_ptr<CThis> n);
      void dispatch(std::shared_ptr<CTrue> n);
      void dispatch(std::shared_ptr<CFalse> n);
      void dispatch(std::shared_ptr<CRef> n);
      void dispatch(std::shared_ptr<CIntegerLiteral> n);
      void dispatch(std::shared_ptr<NewObject> n);
      void dispatch(std::shared_ptr<NewArray> n);
      void dispatch(std::shared_ptr<Equals> n);
      void dispatch(std::shared_ptr<NotEquals> n);
      void dispatch(std::shared_ptr<LessThan> n);
      void dispatch(std::shared_ptr<LessThanOrEqual> n);
      void dispatch(std::shared_ptr<GreaterThan> n);
      void dispatch(std::shared_ptr<GreaterThanOrEqual> n);
      void dispatch(std::shared_ptr<Add> n);
      void dispatch(std::shared_ptr<Subtract> n);
      void dispatch(std::shared_ptr<Multiply> n);
      void dispatch(std::shared_ptr<Divide> n);
      void dispatch(std::shared_ptr<Modulo> n);
      void dispatch(std::shared_ptr<Negate> n);
      void dispatch(std::shared_ptr<Minus> n);
  };
  
  class CollectorError : public SemanticError {
    public:
      CollectorError(const char* err) : SemanticError(err) { }
  };
}
