/*
 * General Checker
 * - checks if there is exactly one main method
 *
 *  is per default valid=true and searches for violation of the upper requirements
 */

#pragma once

#include "ast.h"
#include "checker.h"

namespace cmpl {

  class GeneralChecker : public Dispatcher, public std::enable_shared_from_this<GeneralChecker> {
  private:
    std::shared_ptr<Type> voidNode();
    void error(const std::string &err);
  public:
    int mainMethods=0;
    
    virtual void dispatch(std::shared_ptr<Type> n);
    virtual void dispatch(std::shared_ptr<UserType> n);
    virtual void dispatch(std::shared_ptr<TypeInt> n);
    virtual void dispatch(std::shared_ptr<TypeBoolean> n);
    virtual void dispatch(std::shared_ptr<TypeVoid> n);
    virtual void dispatch(std::shared_ptr<Program> n);
    virtual void dispatch(std::shared_ptr<ClassDeclaration> n);
    virtual void dispatch(std::shared_ptr<Field> n);
    virtual void dispatch(std::shared_ptr<Method> n);
    virtual void dispatch(std::shared_ptr<MainMethod> n);
    virtual void dispatch(std::shared_ptr<Parameter> n);
    virtual void dispatch(std::shared_ptr<Block> n);
    virtual void dispatch(std::shared_ptr<IfStatement> n);
    virtual void dispatch(std::shared_ptr<IfElseStatement> n);
    virtual void dispatch(std::shared_ptr<ExpressionStatement> n);
    virtual void dispatch(std::shared_ptr<WhileStatement> n);
    virtual void dispatch(std::shared_ptr<LocalVariableDeclaration> n);
    virtual void dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n);
    virtual void dispatch(std::shared_ptr<ReturnStatement> n);
    virtual void dispatch(std::shared_ptr<ReturnExpressionStatement> n);
    virtual void dispatch(std::shared_ptr<EmptyStatement> n);
    virtual void dispatch(std::shared_ptr<MethodInvocation> n);
    virtual void dispatch(std::shared_ptr<ArrayAccess> n);
    virtual void dispatch(std::shared_ptr<FieldAccess> n);
    virtual void dispatch(std::shared_ptr<AssignmentExpression> n);
    virtual void dispatch(std::shared_ptr<LogicalOrExpression> n);
    virtual void dispatch(std::shared_ptr<LogicalAndExpression> n);
    virtual void dispatch(std::shared_ptr<EqualityExpression> n);
    virtual void dispatch(std::shared_ptr<RelationalExpression> n);
    virtual void dispatch(std::shared_ptr<AdditiveExpression> n);
    virtual void dispatch(std::shared_ptr<MultiplicativeExpression> n);
    virtual void dispatch(std::shared_ptr<CallExpression> n);
    virtual void dispatch(std::shared_ptr<UnaryLeftExpression> n);
    virtual void dispatch(std::shared_ptr<UnaryRightExpression> n);
    virtual void dispatch(std::shared_ptr<CNull> n);
    virtual void dispatch(std::shared_ptr<CThis> n);
    virtual void dispatch(std::shared_ptr<CTrue> n);
    virtual void dispatch(std::shared_ptr<CFalse> n);
    virtual void dispatch(std::shared_ptr<CRef> n);
    virtual void dispatch(std::shared_ptr<CIntegerLiteral> n);
    virtual void dispatch(std::shared_ptr<NewObject> n);
    virtual void dispatch(std::shared_ptr<NewArray> n);
    virtual void dispatch(std::shared_ptr<Equals> n);
    virtual void dispatch(std::shared_ptr<NotEquals> n);
    virtual void dispatch(std::shared_ptr<LessThan> n);
    virtual void dispatch(std::shared_ptr<LessThanOrEqual> n);
    virtual void dispatch(std::shared_ptr<GreaterThan> n);
    virtual void dispatch(std::shared_ptr<GreaterThanOrEqual> n);
    virtual void dispatch(std::shared_ptr<Add> n);
    virtual void dispatch(std::shared_ptr<Subtract> n);
    virtual void dispatch(std::shared_ptr<Multiply> n);
    virtual void dispatch(std::shared_ptr<Divide> n);
    virtual void dispatch(std::shared_ptr<Modulo> n);
    virtual void dispatch(std::shared_ptr<Negate> n);
    virtual void dispatch(std::shared_ptr<Minus> n);
  };
  
  class MultipleMainMethodsError : public SemanticError
  {
    public:
      MultipleMainMethodsError(const char* err) : SemanticError(err) { }
      
      unsigned int line;
      unsigned int column;
  };

}
