#pragma once

#include "../stringtable/stringtable.h"

#include <memory>
#include <string>
#include <vector>
#include <algorithm>

namespace cmpl
{
  // abstract classes for categorization
  
  class Dispatcher;
  
  class Type;
  class UserType;
  class TypeInt;
  class TypeBoolean;
  class TypeVoid;
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
    virtual void dispatch(Type &n) = 0;
    virtual void dispatch(UserType &n) = 0;
    virtual void dispatch(TypeInt &n) = 0;
    virtual void dispatch(TypeBoolean &n) = 0;
    virtual void dispatch(TypeVoid &n) = 0;
  
    virtual void dispatch(Program &n) = 0;
  
    virtual void dispatch(ClassDeclaration &n) = 0;
  
    virtual void dispatch(Field &n) = 0;
    virtual void dispatch(Method &n) = 0;
    virtual void dispatch(MainMethod &n) = 0;
  
    virtual void dispatch(Parameter &n) = 0;
  
    virtual void dispatch(Block &n) = 0;
  
    virtual void dispatch(IfStatement &n) = 0;
    virtual void dispatch(IfElseStatement &n) = 0;
    virtual void dispatch(ExpressionStatement &n) = 0;
    virtual void dispatch(WhileStatement &n) = 0;
    virtual void dispatch(LocalVariableDeclaration &n) = 0;
    virtual void dispatch(LocalVariableExpressionDeclaration &n) = 0;
    virtual void dispatch(ReturnStatement &n) = 0;
    virtual void dispatch(ReturnExpressionStatement &n) = 0;
    virtual void dispatch(EmptyStatement &n) = 0;
  
    virtual void dispatch(MethodInvocation &n) = 0;
    virtual void dispatch(ArrayAccess &n) = 0;
    virtual void dispatch(FieldAccess &n) = 0;
  
    virtual void dispatch(AssignmentExpression &n) = 0;
    virtual void dispatch(LogicalOrExpression &n) = 0;
    virtual void dispatch(LogicalAndExpression &n) = 0;
    virtual void dispatch(EqualityExpression &n) = 0;
    virtual void dispatch(RelationalExpression &n) = 0;
    virtual void dispatch(AdditiveExpression &n) = 0;
    virtual void dispatch(MultiplicativeExpression &n) = 0;
    virtual void dispatch(CallExpression &n) = 0;
    virtual void dispatch(UnaryLeftExpression &n) = 0;
    virtual void dispatch(UnaryRightExpression &n) = 0;
  
    virtual void dispatch(CNull &n) = 0;
    virtual void dispatch(CThis &n) = 0;
    virtual void dispatch(CTrue &n) = 0;
    virtual void dispatch(CFalse &n) = 0;
    virtual void dispatch(CRef &n) = 0;
    virtual void dispatch(CIntegerLiteral &n) = 0;
    virtual void dispatch(NewObject &n) = 0;
    virtual void dispatch(NewArray &n) = 0;
    
    virtual void dispatch(Equals &n) = 0;
    virtual void dispatch(NotEquals &n) = 0;
    virtual void dispatch(LessThan &n) = 0;
    virtual void dispatch(LessThanOrEqual &n) = 0;
    virtual void dispatch(GreaterThan &n) = 0;
    virtual void dispatch(GreaterThanOrEqual &n) = 0;
    virtual void dispatch(Add &n) = 0;
    virtual void dispatch(Subtract &n) = 0;
    
    virtual void dispatch(Multiply &n) = 0;
    virtual void dispatch(Divide &n) = 0;
    virtual void dispatch(Modulo &n) = 0;
    
    virtual void dispatch(Negate &n) = 0;
    virtual void dispatch(Minus &n) = 0;
  };
  
  class Node
  {
    public:
      virtual void accept(Dispatcher &d) = 0;
  };
  
/**************** actual nodes ****************/

  class BasicType      : public Node           { public: bool isVoid = false; };
  class ClassMember    : public Node           { public: bool returns = false; };
  class Expression     : public Node           { public: };
  class BlockStatement : public Node           { public: bool returns = false; };
  class Statement      : public BlockStatement { public: };
  class Op             : public Node           { public: };
  class EqualityOp     : public Op             { public: };
  class RelationalOp   : public Op             { public: };
  class AddOp          : public Op             { public: };
  class MultOp         : public Op             { public: };
  class UnaryOp        : public Op             { public: };
  
  class Type : public Node
  {
    public:
      std::unique_ptr<BasicType> type;
      int                        arrayDepth;
      
      Type(std::unique_ptr<BasicType> &type, int &arrayDepth) : type(std::move(type)), arrayDepth(arrayDepth) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
    
      bool isVoid = false;
  };
  
  class TypeInt : public BasicType
  {
    public:
      TypeInt() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class TypeBoolean : public BasicType
  {
    public:
      TypeBoolean() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class TypeVoid : public BasicType
  {
    public:
      TypeVoid() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class UserType : public BasicType
  {
    public:
      StringIdentifier ID;
      
      UserType(StringIdentifier &ID) : ID(ID) { }
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class NotEquals : public EqualityOp
  {
    public:
      NotEquals() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class Equals : public EqualityOp
  {
    public:
      Equals() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class LessThan : public RelationalOp
  {
    public:
      LessThan() { }
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class LessThanOrEqual : public RelationalOp
  {
    public:
      LessThanOrEqual() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class GreaterThan : public RelationalOp
  {
    public:
      GreaterThan() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class GreaterThanOrEqual : public RelationalOp
  {
    public:
      GreaterThanOrEqual() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class Add : public AddOp
  {
    public:
      Add() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class Subtract : public AddOp
  {
    public:
      Subtract() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class Multiply : public MultOp
  {
    public:
      Multiply() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class Divide : public MultOp
  {
    public:
      Divide() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class Modulo : public MultOp
  {
    public:
      Modulo() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class Negate : public UnaryOp
  {
    public:
      Negate() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class Minus : public UnaryOp
  {
    public:
      Minus() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class MethodInvocation : public UnaryOp
  {
    public:
      StringIdentifier                         ID;
      std::vector<std::unique_ptr<Expression>> arguments;
      
      MethodInvocation(StringIdentifier &ID, std::vector<std::unique_ptr<Expression>> &arguments) :
                         ID(ID), arguments(std::move(arguments)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class FieldAccess : public UnaryOp
  {
    public:
      StringIdentifier ID;
      
      FieldAccess(StringIdentifier &ID) : ID(ID) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class ArrayAccess : public UnaryOp
  {
    public:
      std::unique_ptr<Expression> expression;
      
      ArrayAccess(std::unique_ptr<Expression> &expression) : expression(std::move(expression)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  
/*********************** Expressions ***********************/
  
  class OpExpression : public Expression
  {
    protected:
      OpExpression(std::unique_ptr<Op> op, std::unique_ptr<Expression> expression1,
                   std::unique_ptr<Expression> expression2) :
                     op(std::move(op)), expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    public:
      std::unique_ptr<Op>         op;
      std::unique_ptr<Expression> expression1;
      std::unique_ptr<Expression> expression2;
  };
  
  class AssignmentExpression : public Expression
  {
    public:
      std::unique_ptr<Expression> expression1;
      std::unique_ptr<Expression> expression2;
      
      AssignmentExpression(std::unique_ptr<Expression> &expression1, std::unique_ptr<Expression> &expression2) :
                             expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class LogicalOrExpression : public Expression
  {
    public:
      std::unique_ptr<Expression> expression1;
      std::unique_ptr<Expression> expression2;
      
      LogicalOrExpression(std::unique_ptr<Expression> &expression1, std::unique_ptr<Expression> &expression2) :
                            expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class LogicalAndExpression : public Expression
  {
    public:
      std::unique_ptr<Expression> expression1;
      std::unique_ptr<Expression> expression2;
      
      LogicalAndExpression(std::unique_ptr<Expression> &expression1, std::unique_ptr<Expression> &expression2) :
                             expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class EqualityExpression : public OpExpression
  {
    public:
      EqualityExpression(std::unique_ptr<EqualityOp> &op, std::unique_ptr<Expression> &expression1,
                         std::unique_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };

  class RelationalExpression : public OpExpression
  {
    public:
      RelationalExpression(std::unique_ptr<RelationalOp> &op, std::unique_ptr<Expression> &expression1,
                           std::unique_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class AdditiveExpression : public OpExpression
  {
    public:
      AdditiveExpression(std::unique_ptr<AddOp> &op, std::unique_ptr<Expression> &expression1,
                         std::unique_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)){ };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class MultiplicativeExpression : public OpExpression
  {
    public:
      MultiplicativeExpression(std::unique_ptr<MultOp> &op, std::unique_ptr<Expression> &expression1,
                               std::unique_ptr<Expression> &expression2) : 
                                 OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class CallExpression : public Expression
  {
    public:
      StringIdentifier                         ID;
      std::vector<std::unique_ptr<Expression>> arguments;
      
      CallExpression(StringIdentifier &ID, std::vector<std::unique_ptr<Expression>> &arguments) :
                       ID(ID), arguments(std::move(arguments)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class UnaryLeftExpression : public Expression
  {
    public:
      std::unique_ptr<UnaryOp>    op;
      std::unique_ptr<Expression> expression;
      
      UnaryLeftExpression(std::unique_ptr<UnaryOp> &op, std::unique_ptr<Expression> &expression) :
                            op(std::move(op)), expression(std::move(expression)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class UnaryRightExpression : public Expression
  {
    public:
      std::unique_ptr<UnaryOp>    op;
      std::unique_ptr<Expression> expression;
      
      UnaryRightExpression(std::unique_ptr<Expression> &expression, std::unique_ptr<UnaryOp> &op) :
                             expression(std::move(expression)), op(std::move(op)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class CNull : public Expression
  {
    public:
      CNull() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class CFalse : public Expression
  {
    public:
      CFalse() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class CTrue : public Expression
  {
    public:
      CTrue() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class CThis : public Expression
  {
    public:
      CThis() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class CIntegerLiteral : public Expression
  {
    public:
      std::string integer;
      
      CIntegerLiteral(std::string &integer) : integer(integer) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class CRef : public Expression
  {
    public:
      StringIdentifier ID;
      
      CRef(StringIdentifier &ID) : ID(ID) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class NewObject : public Expression
  {
    public:
      StringIdentifier ID;
      
      NewObject(StringIdentifier &ID) : ID(ID) { };

      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class NewArray : public Expression
  {
    public:
      std::unique_ptr<BasicType>  type;
      std::unique_ptr<Expression> expression;
      int                         arrayDepth;
      
      NewArray(std::unique_ptr<BasicType>  &type, std::unique_ptr<Expression> &expression,
               int &arrayDepth) :
                 type(std::move(type)), expression(std::move(expression)), arrayDepth(arrayDepth) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class Parameter : public Node
  {
    public:
      std::unique_ptr<Type> type;
      StringIdentifier      ID;
      
      Parameter(std::unique_ptr<Type> &type, StringIdentifier &ID) :
            type(std::move(type)), ID(ID) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  
/*********************** Statements ***********************/
  
  
  class Block : public Statement
  {
    public:
      std::vector<std::unique_ptr<BlockStatement>> statements;
      
      Block(std::vector<std::unique_ptr<BlockStatement>> &statements) : statements(std::move(statements)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class IfStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      std::unique_ptr<Statement>  ifStatement;
      
      IfStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &ifStatement) :
                    expression(std::move(expression)), ifStatement(std::move(ifStatement)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class IfElseStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      std::unique_ptr<Statement>  ifStatement;
      std::unique_ptr<Statement>  elseStatement;
      
      IfElseStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &ifStatement,
                      std::unique_ptr<Statement> &elseStatement) :
                        expression(std::move(expression)), ifStatement(std::move(ifStatement)),
                        elseStatement(std::move(elseStatement)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class ExpressionStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      
      ExpressionStatement(std::unique_ptr<Expression> &expression) :
                            expression(std::move(expression)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class WhileStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      std::unique_ptr<Statement>  statement;
      
      WhileStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &statement) :
                       expression(std::move(expression)), statement(std::move(statement)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class ReturnExpressionStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression; 
      
      ReturnExpressionStatement(std::unique_ptr<Expression> &expression) :
                                  expression(std::move(expression)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class ReturnStatement : public Statement
  {
    public:
      ReturnStatement() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class EmptyStatement : public Statement
  {
    public:
      EmptyStatement() { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class LocalVariableDeclaration : public BlockStatement
  {
    public:
      std::unique_ptr<Type> type;
      StringIdentifier      ID;
      
      LocalVariableDeclaration(std::unique_ptr<Type> &type, StringIdentifier &ID) :
                                 type(std::move(type)), ID(ID) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class LocalVariableExpressionDeclaration : public BlockStatement
  {
    public:
      std::unique_ptr<Type>       type;
      StringIdentifier            ID;
      std::unique_ptr<Expression> expression;
      
      LocalVariableExpressionDeclaration(std::unique_ptr<Type> &type, StringIdentifier &ID,
                                         std::unique_ptr<Expression> &expression) :
                                           type(std::move(type)), ID(ID), expression(std::move(expression)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  
/*********************** Class related ***********************/
  
  
  class Field : public ClassMember
  {
    public:
      std::unique_ptr<Type> type;
      StringIdentifier      ID;
      
      Field(std::unique_ptr<Type> &type, StringIdentifier &ID) :
            type(std::move(type)), ID(ID) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  class Method : public ClassMember
  {
    public:
      std::unique_ptr<Type>                   type;
      StringIdentifier                        ID;
      std::vector<std::unique_ptr<Parameter>> parameters;
      std::unique_ptr<Block>                  block;
      
      Method(std::unique_ptr<Type> &type, StringIdentifier &ID, std::vector<std::unique_ptr<Parameter>> &parameters,
             std::unique_ptr<Block> &block) :
               type(std::move(type)), ID(ID), parameters(std::move(parameters)), block(std::move(block)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  /** The MainMethod is a ClassMember and therefore part of a ClassDeclaration. The MainMethod element contains two IDs (Method-identifier and parameter-identifier) and a Block (Representation of the method body) */
  class MainMethod : public ClassMember
  {
    public:
      StringIdentifier       ID;
      StringIdentifier       parameterID;
      std::unique_ptr<Block> block;
      
      MainMethod(StringIdentifier &ID, StringIdentifier &parameterID, std::unique_ptr<Block> &block) :
                   ID(ID), parameterID(std::move(parameterID)), block(std::move(block)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  /** A ClassDeclaration consists of an ID, which is the identifier for this class and various ClassMembers, like Methods and Fields.  */
  class ClassDeclaration : public Node
  {
    public:
      StringIdentifier                          ID;
      std::vector<std::unique_ptr<ClassMember>> classMembers;
      std::map<StringIdentifier, std::weak_ptr<Method>> methods;
      std::map<StringIdentifier, std::weak_ptr<Field>> fields;
    
      bool returns = false;
    
      ClassDeclaration(StringIdentifier &ID, std::vector<std::unique_ptr<ClassMember>> &classMembers) :
                         ID(ID), classMembers(std::move(classMembers)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  /** Always the first element in the AST. Can contain multiple ClassDeclarations. */	
  class Program : public Node
  {
    public:
      std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;
      
      Program(std::vector<std::unique_ptr<ClassDeclaration>> &classDeclarations) :
                classDeclarations(std::move(classDeclarations)) { };
    
      void accept (Dispatcher& d) override {
        d.dispatch(*this);
      }
  };
  
  
}
