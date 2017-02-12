/*
 * General Checker
 * - checks that there is exactly one main method
 * - checks that this is not used
 *
 *  is per default valid=true and searches for violation of the upper requirements
 */

#pragma once

#include "checker.h"

namespace cmpl {

  class MainMethodChecker : public Dispatcher, public std::enable_shared_from_this<MainMethodChecker> {
  private:
    void error(const std::string &err);
  public:
    int mainMethods=0;
    std::shared_ptr<MainMethod> currentMainMethod;
    std::shared_ptr<ClassDeclaration> currentClassDeclaration;
    
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
    void dispatch(std::shared_ptr<SLCPrintlnExpression> n);
    void dispatch(std::shared_ptr<SLCWriteExpression> n);
    void dispatch(std::shared_ptr<SLCFlushExpression> n);
    void dispatch(std::shared_ptr<SLCReadExpression> n);
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
  
  class MainMethodError : public SemanticError
  {
    public:
      MainMethodError(const char* err) : SemanticError(err) { }
  };
}
