//
//  Static Resolver
//  Der Static Resolver löst alle Verwender vom AST-Typ CRef und UserType auf
//  "Static" deshalb, weil für CRef und UserType keine vorhergehende Typanalyse notwendig ist
//  Benötigt einen Durchlauf des Static Declarations Collector als Voraussetzung.
//  Ist Voraussetzung für eine ausführliche Typanalyse, weil für diese alle UserTypes
//  aufgelöst sein müssen.
//  Es wäre möglich, hier auch CallExpression zu behandeln. Wegen Konsistenz würde ich das
//  aber erst in einem späteren Schritt zusammen mit MethodInvocation und FieldAccess
//  (die beide eine vorhergehende Typanalyse brauchen) erledigen.
//

#pragma once

#include "node.h"
#include "symboltable.h"

using namespace std;

namespace cmpl {
  
  class StaticResolver : public Dispatcher, public std::enable_shared_from_this<StaticResolver> {
  private:
    std::shared_ptr<Program> currentProgram;
    std::shared_ptr<ClassDeclaration> currentClassDeclaration;
    std::shared_ptr<Method> currentMethod;
    std::unique_ptr<SymbolTable> currentSymbolTable;
    bool valid = false;
    
  public:
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
  
}
