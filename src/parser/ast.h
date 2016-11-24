#pragma once

#include "../stringtable/stringtable.h"

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>

namespace cmpl
{
  // abstract classes for categorization
  
  class Dispatcher;
  
  class Type;
  class TypedNode;
  class TypeBoolean;
  class TypeInt;
  class TypeVoid;
  class UserType;
  class Program;
  class ClassDeclaration;
  class Field;
  class Method;
  class MainMethod;
  class Parameter;
  class Block;
  class IfStatement;
  class IfElseStatement;
  class ExpressionStatement;
  class WhileStatement;
  class LocalVariableDeclaration;
  class LocalVariableExpressionDeclaration;
  class ReturnStatement;
  class ReturnExpressionStatement;
  class EmptyStatement;
  class MethodInvocation;
  class ArrayAccess;
  class FieldAccess;
  class AssignmentExpression;
  class LogicalOrExpression;
  class LogicalAndExpression;
  class EqualityExpression;
  class RelationalExpression;
  class AdditiveExpression;
  class MultiplicativeExpression;
  class CallExpression;
  class UnaryLeftExpression;
  class UnaryRightExpression;
  class CNull;
  class CThis;
  class CTrue;
  class CFalse;
  class CRef;
  class CIntegerLiteral;
  class NewObject;
  class NewArray;
  class Equals;
  class NotEquals;
  class LessThan;
  class LessThanOrEqual;
  class GreaterThan;
  class GreaterThanOrEqual;
  class Add;
  class Subtract;
  class Multiply;
  class Divide;
  class Modulo;
  class Negate;
  class Minus;
  
  // ------ Visitor pattern ------ //
  class Dispatcher {
  public:
    virtual void dispatch(std::shared_ptr<Type> n) = 0;
    virtual void dispatch(std::shared_ptr<TypeBoolean> n) = 0;
    virtual void dispatch(std::shared_ptr<TypeInt> n) = 0;
    virtual void dispatch(std::shared_ptr<TypeVoid> n) = 0;
    virtual void dispatch(std::shared_ptr<UserType> n) = 0;
    virtual void dispatch(std::shared_ptr<Program> n) = 0;
    virtual void dispatch(std::shared_ptr<ClassDeclaration> n) = 0;
    virtual void dispatch(std::shared_ptr<Field> n) = 0;
    virtual void dispatch(std::shared_ptr<Method> n) = 0;
    virtual void dispatch(std::shared_ptr<MainMethod> n) = 0;
    virtual void dispatch(std::shared_ptr<Parameter> n) = 0;
    virtual void dispatch(std::shared_ptr<Block> n) = 0;
    virtual void dispatch(std::shared_ptr<IfStatement> n) = 0;
    virtual void dispatch(std::shared_ptr<IfElseStatement> n) = 0;
    virtual void dispatch(std::shared_ptr<ExpressionStatement> n) = 0;
    virtual void dispatch(std::shared_ptr<WhileStatement> n) = 0;
    virtual void dispatch(std::shared_ptr<LocalVariableDeclaration> n) = 0;
    virtual void dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) = 0;
    virtual void dispatch(std::shared_ptr<ReturnStatement> n) = 0;
    virtual void dispatch(std::shared_ptr<ReturnExpressionStatement> n) = 0;
    virtual void dispatch(std::shared_ptr<EmptyStatement> n) = 0;
    virtual void dispatch(std::shared_ptr<MethodInvocation> n) = 0;
    virtual void dispatch(std::shared_ptr<ArrayAccess> n) = 0;
    virtual void dispatch(std::shared_ptr<FieldAccess> n) = 0;
    virtual void dispatch(std::shared_ptr<AssignmentExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<LogicalOrExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<LogicalAndExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<EqualityExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<RelationalExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<AdditiveExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<MultiplicativeExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<CallExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<UnaryLeftExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<UnaryRightExpression> n) = 0;
    virtual void dispatch(std::shared_ptr<CNull> n) = 0;
    virtual void dispatch(std::shared_ptr<CThis> n) = 0;
    virtual void dispatch(std::shared_ptr<CTrue> n) = 0;
    virtual void dispatch(std::shared_ptr<CFalse> n) = 0;
    virtual void dispatch(std::shared_ptr<CRef> n) = 0;
    virtual void dispatch(std::shared_ptr<CIntegerLiteral> n) = 0;
    virtual void dispatch(std::shared_ptr<NewObject> n) = 0;
    virtual void dispatch(std::shared_ptr<NewArray> n) = 0;
    virtual void dispatch(std::shared_ptr<Equals> n) = 0;
    virtual void dispatch(std::shared_ptr<NotEquals> n) = 0;
    virtual void dispatch(std::shared_ptr<LessThan> n) = 0;
    virtual void dispatch(std::shared_ptr<LessThanOrEqual> n) = 0;
    virtual void dispatch(std::shared_ptr<GreaterThan> n) = 0;
    virtual void dispatch(std::shared_ptr<GreaterThanOrEqual> n) = 0;
    virtual void dispatch(std::shared_ptr<Add> n) = 0;
    virtual void dispatch(std::shared_ptr<Subtract> n) = 0;
    virtual void dispatch(std::shared_ptr<Multiply> n) = 0;
    virtual void dispatch(std::shared_ptr<Divide> n) = 0;
    virtual void dispatch(std::shared_ptr<Modulo> n) = 0;
    virtual void dispatch(std::shared_ptr<Negate> n) = 0;
    virtual void dispatch(std::shared_ptr<Minus> n) = 0;
  };
  
  class Node
  {
    public:
      virtual void accept(std::shared_ptr<Dispatcher> d) = 0;
  };
  
/**************** actual nodes ****************/
  class BasicType : public Node { public: bool virtual equals(std::shared_ptr<BasicType> t) = 0; };
  class TypedNode
  {
    public:
      TypedNode() { };
      TypedNode(std::shared_ptr<Type> type) : type(type) { };
      TypedNode(std::shared_ptr<BasicType> basicType, int arrayDepth) : type(std::make_shared<Type>(basicType, arrayDepth)) { };
      
      // Explanation to lvalues and rvalues: (initialized to false)
      // http://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c
      bool isLValue = false;
      std::shared_ptr<Type> type;
  };
  
  class ClassMember    : public Node                 { };
  class BlockStatement : public Node                 { public: bool returns = false; };
  class Statement      : public BlockStatement       { };
  class Op             : public Node                 { };
  class EqualityOp     : public Op                   { };
  class RelationalOp   : public Op                   { };
  class AddOp          : public Op                   { };
  class MultOp         : public Op                   { };
  class UnaryOp        : public Op, public TypedNode { };
  
  
  // helper classes for typechecker
  
  class Type : public Node, public std::enable_shared_from_this<Type>
  {
    public:
      std::shared_ptr<BasicType> basicType;
      int                        arrayDepth;
      
      Type(std::shared_ptr<BasicType> const& basicType, int const& arrayDepth) : basicType(std::move(basicType)), arrayDepth(arrayDepth) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
    
      bool equals(std::shared_ptr<Type> t) {
        return shared_from_this()->arrayDepth == t->arrayDepth && shared_from_this()->basicType->equals(t->basicType);
      }
  };
  
  // basic types (created in AST)
  
  class TypeInt : public BasicType, public std::enable_shared_from_this<TypeInt>
  {
    public:
      TypeInt() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
    
      bool equals(std::shared_ptr<BasicType> t) override {
        if (dynamic_cast<TypeInt*>(t.get())) {
          return true;
        } else {
          return false;
        }
      }
  };
  
  class TypeBoolean : public BasicType, public std::enable_shared_from_this<TypeBoolean>
  {
    public:
      TypeBoolean() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
    
      bool equals(std::shared_ptr<BasicType> t) override {
        if (dynamic_cast<TypeBoolean*>(t.get())) {
          return true;
        } else {
          return false;
        }
      }
  };
  
  class TypeVoid : public BasicType, public std::enable_shared_from_this<TypeVoid>
  {
    public:
      TypeVoid() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
    
      bool equals(std::shared_ptr<BasicType> t) override {
        if (dynamic_cast<TypeVoid*>(t.get())) {
          return true;
        } else {
          return false;
        }
      }
  };
  
  class UserType : public BasicType, public std::enable_shared_from_this<UserType>
  {
    public:
      StringIdentifier ID;
      std::weak_ptr<ClassDeclaration> declaration;
      
      UserType(StringIdentifier &ID) : ID(ID) { }
      
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
    
      bool equals(std::shared_ptr<BasicType> other) override {
        if (UserType* ut = dynamic_cast<UserType*>(other.get())) {
          return ID == ut->ID;
        } else {
          return false;
      }
    }
    
    bool operator!= (std::shared_ptr<UserType> t) {
      return !(shared_from_this() == t);
    }
  };
  
  // other nodes
  
  class Expression : public Node, public TypedNode
  {
  public:
    Expression() { };
    // necessary to allow NewArray set attributes of TypedNode in its contructor
    Expression(std::shared_ptr<BasicType> basicType, int arrayDepth) : TypedNode(basicType, arrayDepth) { };
    bool isValidSemanticType() { // Semantic types type expressions, expressions cannot be void
      if (TypeVoid *v = dynamic_cast<TypeVoid*>(type->basicType.get())) {
        return true;
      } else {
        return false;
      }
    }
  };

  class NotEquals : public EqualityOp, public std::enable_shared_from_this<NotEquals>
  {
    public:
      NotEquals() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class Equals : public EqualityOp, public std::enable_shared_from_this<Equals>
  {
    public:
      Equals() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class LessThan : public RelationalOp, public std::enable_shared_from_this<LessThan>
  {
    public:
      LessThan() { }
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class LessThanOrEqual : public RelationalOp, public std::enable_shared_from_this<LessThanOrEqual>
  {
    public:
      LessThanOrEqual() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class GreaterThan : public RelationalOp, public std::enable_shared_from_this<GreaterThan>
  {
    public:
      GreaterThan() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class GreaterThanOrEqual : public RelationalOp, public std::enable_shared_from_this<GreaterThanOrEqual>
  {
    public:
      GreaterThanOrEqual() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class Add : public AddOp, public std::enable_shared_from_this<Add>
  {
    public:
      Add() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class Subtract : public AddOp, public std::enable_shared_from_this<Subtract>
  {
    public:
      Subtract() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class Multiply : public MultOp, public std::enable_shared_from_this<Multiply>
  {
    public:
      Multiply() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class Divide : public MultOp, public std::enable_shared_from_this<Divide>
  {
    public:
      Divide() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class Modulo : public MultOp, public std::enable_shared_from_this<Modulo>
  {
    public:
      Modulo() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class Negate : public UnaryOp, public std::enable_shared_from_this<Negate>
  {
    public:
      Negate() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class Minus : public UnaryOp, public std::enable_shared_from_this<Minus>
  {
    public:
      Minus() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class MethodInvocation : public UnaryOp, public std::enable_shared_from_this<MethodInvocation>
  {
    public:
      StringIdentifier                         ID;
      std::vector<std::shared_ptr<Expression>> arguments;
      
      MethodInvocation(StringIdentifier &ID, std::vector<std::shared_ptr<Expression>> &arguments) :
                         ID(ID), arguments(std::move(arguments)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class FieldAccess : public UnaryOp, public std::enable_shared_from_this<FieldAccess>
  {
    public:
      StringIdentifier ID;
      
      FieldAccess(StringIdentifier &ID) : ID(ID) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class ArrayAccess : public UnaryOp, public std::enable_shared_from_this<ArrayAccess>
  {
    public:
      std::shared_ptr<Expression> expression;
      
      ArrayAccess(std::shared_ptr<Expression> &expression) : expression(std::move(expression)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  
/*********************** Expressions ***********************/
  
  class OpExpression : public Expression
  {
    protected:
      OpExpression(std::shared_ptr<Op> op, std::shared_ptr<Expression> expression1,
                   std::shared_ptr<Expression> expression2) :
                     op(std::move(op)), expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    public:
      std::shared_ptr<Op>         op;
      std::shared_ptr<Expression> expression1;
      std::shared_ptr<Expression> expression2;
  };
  
  class AssignmentExpression : public Expression, public std::enable_shared_from_this<AssignmentExpression>
  {
    public:
      std::shared_ptr<Expression> expression1;
      std::shared_ptr<Expression> expression2;
      
      AssignmentExpression(std::shared_ptr<Expression> &expression1, std::shared_ptr<Expression> &expression2) :
                             expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class LogicalOrExpression : public Expression, public std::enable_shared_from_this<LogicalOrExpression>
  {
    public:
      std::shared_ptr<Expression> expression1;
      std::shared_ptr<Expression> expression2;
      
      LogicalOrExpression(std::shared_ptr<Expression> &expression1, std::shared_ptr<Expression> &expression2) :
                            expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class LogicalAndExpression : public Expression, public std::enable_shared_from_this<LogicalAndExpression>
  {
    public:
      std::shared_ptr<Expression> expression1;
      std::shared_ptr<Expression> expression2;
      
      LogicalAndExpression(std::shared_ptr<Expression> &expression1, std::shared_ptr<Expression> &expression2) :
                             expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class EqualityExpression : public OpExpression, public std::enable_shared_from_this<EqualityExpression>
  {
    public:
      EqualityExpression(std::shared_ptr<EqualityOp> &op, std::shared_ptr<Expression> &expression1,
                         std::shared_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };

  class RelationalExpression : public OpExpression, public std::enable_shared_from_this<RelationalExpression>
  {
    public:
      RelationalExpression(std::shared_ptr<RelationalOp> &op, std::shared_ptr<Expression> &expression1,
                           std::shared_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class AdditiveExpression : public OpExpression, public std::enable_shared_from_this<AdditiveExpression>
  {
    public:
      AdditiveExpression(std::shared_ptr<AddOp> &op, std::shared_ptr<Expression> &expression1,
                         std::shared_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)){ };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class MultiplicativeExpression : public OpExpression, public std::enable_shared_from_this<MultiplicativeExpression>
  {
    public:
      MultiplicativeExpression(std::shared_ptr<MultOp> &op, std::shared_ptr<Expression> &expression1,
                               std::shared_ptr<Expression> &expression2) : 
                                 OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class CallExpression : public Expression, public std::enable_shared_from_this<CallExpression>
  {
    public:
      StringIdentifier                         ID;
      std::vector<std::shared_ptr<Expression>> arguments;
      std::weak_ptr<Method> declaration;
    
      CallExpression(StringIdentifier &ID, std::vector<std::shared_ptr<Expression>> &arguments) :
                       ID(ID), arguments(std::move(arguments)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class UnaryLeftExpression : public Expression, public std::enable_shared_from_this<UnaryLeftExpression>
  {
    public:
      std::shared_ptr<UnaryOp>    op;
      std::shared_ptr<Expression> expression;
      
      UnaryLeftExpression(std::shared_ptr<UnaryOp> &op, std::shared_ptr<Expression> &expression) :
                            op(std::move(op)), expression(std::move(expression)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class UnaryRightExpression : public Expression, public std::enable_shared_from_this<UnaryRightExpression>
  {
    public:
      std::shared_ptr<UnaryOp>    op;
      std::shared_ptr<Expression> expression;
      
      UnaryRightExpression(std::shared_ptr<Expression> &expression, std::shared_ptr<UnaryOp> &op) :
                             expression(std::move(expression)), op(std::move(op)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class CNull : public Expression, public std::enable_shared_from_this<CNull>
  {
    public:
      CNull() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class CFalse : public Expression, public std::enable_shared_from_this<CFalse>
  {
    public:
      CFalse() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class CTrue : public Expression, public std::enable_shared_from_this<CTrue>
  {
    public:
      CTrue() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class CThis : public Expression, public std::enable_shared_from_this<CThis>
  {
    public:
      std::weak_ptr<Node> declaration;
    
      CThis() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class CIntegerLiteral : public Expression, public std::enable_shared_from_this<CIntegerLiteral>
  {
    public:
      std::string integer;
      uint32_t value;
      
      CIntegerLiteral(std::string &integer) : integer(integer) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class CRef : public Expression, public std::enable_shared_from_this<CRef>
  {
    public:
      StringIdentifier ID;
      std::weak_ptr<Node> declaration;
    
      CRef(StringIdentifier &ID) : ID(ID) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class NewObject : public Expression, public std::enable_shared_from_this<NewObject>
  {
    public:
      std::shared_ptr<UserType> userType;
    
      NewObject(std::shared_ptr<UserType> t) : userType(t) { };

      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class NewArray : public Expression, public std::enable_shared_from_this<NewArray>
  {
    public:
      std::shared_ptr<Expression> expression;
      
      NewArray(std::shared_ptr<BasicType> type, std::shared_ptr<Expression> &expression,
               int &arrayDepth) : Expression(type, arrayDepth), expression(std::move(expression)) {
      };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class Parameter : public Node, public TypedNode, public std::enable_shared_from_this<Parameter>
  {
    public:
      StringIdentifier ID;
      
      Parameter(std::shared_ptr<Type> type, StringIdentifier ID) :
            TypedNode(type), ID(ID) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  
/*********************** Statements ***********************/
  
  
  class Block : public Statement, public std::enable_shared_from_this<Block>
  {
    public:
      std::vector<std::shared_ptr<BlockStatement>> statements;
	 
      Block(std::vector<std::shared_ptr<BlockStatement>> &statements) : statements(std::move(statements)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class IfStatement : public Statement, public std::enable_shared_from_this<IfStatement>
  {
    public:
      std::shared_ptr<Expression> expression;
      std::shared_ptr<Statement>  ifStatement;
      
      IfStatement(std::shared_ptr<Expression> &expression, std::shared_ptr<Statement> &ifStatement) :
                    expression(std::move(expression)), ifStatement(std::move(ifStatement)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class IfElseStatement : public Statement, public std::enable_shared_from_this<IfElseStatement>
  {
    public:
      std::shared_ptr<Expression> expression;
      std::shared_ptr<Statement>  ifStatement;
      std::shared_ptr<Statement>  elseStatement;
      
      IfElseStatement(std::shared_ptr<Expression> &expression, std::shared_ptr<Statement> &ifStatement,
                      std::shared_ptr<Statement> &elseStatement) :
                        expression(std::move(expression)), ifStatement(std::move(ifStatement)),
                        elseStatement(std::move(elseStatement)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class ExpressionStatement : public Statement, public std::enable_shared_from_this<ExpressionStatement>
  {
    public:
      std::shared_ptr<Expression> expression;
      
      ExpressionStatement(std::shared_ptr<Expression> &expression) :
                            expression(std::move(expression)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class WhileStatement : public Statement, public std::enable_shared_from_this<WhileStatement>
  {
    public:
      std::shared_ptr<Expression> expression;
      std::shared_ptr<Statement>  statement;
      
      WhileStatement(std::shared_ptr<Expression> &expression, std::shared_ptr<Statement> &statement) :
                       expression(std::move(expression)), statement(std::move(statement)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class ReturnExpressionStatement : public Statement, public std::enable_shared_from_this<ReturnExpressionStatement>
  {
    public:
      std::shared_ptr<Expression> expression; 
      
      ReturnExpressionStatement(std::shared_ptr<Expression> &expression) :
                                  expression(std::move(expression)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class ReturnStatement : public Statement, public std::enable_shared_from_this<ReturnStatement>
  {
    public:
      ReturnStatement() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class EmptyStatement : public Statement, public std::enable_shared_from_this<EmptyStatement>
  {
    public:
      EmptyStatement() { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class LocalVariableDeclaration : public BlockStatement, public TypedNode, public std::enable_shared_from_this<LocalVariableDeclaration>
  {
    public:
      StringIdentifier ID;
      
      LocalVariableDeclaration(std::shared_ptr<Type> &type, StringIdentifier &ID) :
                                 TypedNode(type), ID(ID) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class LocalVariableExpressionDeclaration : public BlockStatement, public TypedNode, public std::enable_shared_from_this<LocalVariableExpressionDeclaration>
  {
    public:
      StringIdentifier            ID;
      std::shared_ptr<Expression> expression;
      
      LocalVariableExpressionDeclaration(std::shared_ptr<Type> &type, StringIdentifier &ID,
                                         std::shared_ptr<Expression> &expression) :
                                           TypedNode(type), ID(ID), expression(std::move(expression)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  
/*********************** Class related ***********************/
  
  
  class Field : public ClassMember, public TypedNode, public std::enable_shared_from_this<Field>
  {
    public:
      StringIdentifier      ID;
      
      Field(std::shared_ptr<Type> &type, StringIdentifier &ID) :
            TypedNode(type), ID(ID) { };
      
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  class Method : public ClassMember, public TypedNode, public std::enable_shared_from_this<Method>
  {
    public:
      StringIdentifier                        ID;
      std::vector<std::shared_ptr<Parameter>> parameters;
      std::vector<std::shared_ptr<Node>>      localVariables;
      std::shared_ptr<Block>                  block;
      std::map<StringIdentifier, std::weak_ptr<Parameter>> parameterMap;
      
      Method(std::shared_ptr<Type> &type, StringIdentifier &ID, std::vector<std::shared_ptr<Parameter>> &parameters,
             std::shared_ptr<Block> &block) :
               TypedNode(type), ID(ID), parameters(std::move(parameters)), block(std::move(block)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  /** The MainMethod is a ClassMember and therefore part of a ClassDeclaration. The MainMethod element contains two IDs (Method-identifier and parameter-identifier) and a Block (Representation of the method body) */
  class MainMethod : public ClassMember, public std::enable_shared_from_this<MainMethod>
  {
    public:
      StringIdentifier       ID;
      StringIdentifier       parameterID;
      std::shared_ptr<Block> block;
      std::vector<std::shared_ptr<Node>>      localVariables;
    
      MainMethod(StringIdentifier &ID, StringIdentifier &parameterID, std::shared_ptr<Block> &block) :
                   ID(ID), parameterID(std::move(parameterID)), block(std::move(block)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  

  /** A ClassDeclaration consists of an ID, which is the identifier for this class and various ClassMembers, like Methods and Fields.  */
  class ClassDeclaration : public Node, public TypedNode, public std::enable_shared_from_this<ClassDeclaration>
  {
    public:
      StringIdentifier                          ID;
      std::vector<std::shared_ptr<ClassMember>> classMembers;
      std::map<StringIdentifier, std::weak_ptr<Method>> methods;
      std::map<StringIdentifier, std::weak_ptr<Field>> fields;
      
      bool returns = false;
      
      ClassDeclaration(StringIdentifier &ID, std::vector<std::shared_ptr<ClassMember>> &classMembers) :
                         ID(ID), TypedNode(std::make_shared<UserType>(ID), 0), classMembers(std::move(classMembers)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  /** Always the first element in the AST. Can contain multiple ClassDeclarations. */	
  class Program : public Node, public std::enable_shared_from_this<Program>
  {
    public:
      std::vector<std::shared_ptr<ClassDeclaration>> classDeclarations;
     
      Program(std::vector<std::shared_ptr<ClassDeclaration>> &classDeclarations) :
                classDeclarations(std::move(classDeclarations)) { };
    
      void accept (std::shared_ptr<Dispatcher> d) override {
        d->dispatch(shared_from_this());
      }
  };
  
  
}
