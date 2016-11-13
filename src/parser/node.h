#pragma once

#include "prettyprinter.h"
#include "../stringtable/stringtable.h"

#include <memory>
#include <string>
#include <vector>
#include <algorithm>

namespace cmpl
{
  // abstract classes for categorization
  class Node
  {
    public:
      virtual void toString(PrettyPrinter &printer) const = 0; // must be implemented by subclasses
  };
  
/**************** actual nodes ****************/

  class BasicType    : public Node { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class ClassMember  : public Node { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class Expression   : public Node { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class Statement    : public Node { public: virtual void toString(PrettyPrinter &printer) const = 0; };

  class Op           : public Node { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class EqualityOp   : public Op   { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class RelationalOp : public Op   { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class AddOp        : public Op   { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class MultOp       : public Op   { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class UnaryOp      : public Op   { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  
  class Type : public Node
  {
    public:
      std::unique_ptr<BasicType> type;
      int                        arrayDepth;
      
      Type(std::unique_ptr<BasicType> &type, int &arrayDepth) : type(std::move(type)), arrayDepth(arrayDepth) { };
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        for(int i=0; i<arrayDepth; i++) {
          printer.print("[]");
        }
      };
  };
  
  class TypeInt : public BasicType
  {
    public:
      TypeInt() { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("int");
      };
  };
  
  class TypeBoolean : public BasicType
  {
    public:
      TypeBoolean() { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("boolean");
      };
  };
  
  class TypeVoid : public BasicType
  {
    public:
      TypeVoid() { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("void");
      };
  };
  
  class UserType : public BasicType
  {
    public:
      std::unique_ptr<IdentifierToken> ID;
      
      UserType(std::unique_ptr<IdentifierToken> &ID) : ID(std::move(ID)) { }
      
      void toString(PrettyPrinter &printer) const {
        printer.print(ID->getIdentifier());
      };
  };

  class NotEquals : public EqualityOp
  {
    public:
      NotEquals() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("!=");
      };
  };

  class Equals : public EqualityOp
  {
    public:
      Equals() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("==");
      };
  };

  class LessThan : public RelationalOp
  {
    public:
      LessThan() { }
      
      void toString(PrettyPrinter &printer) const {
        printer.print("<");
      };
  };

  class LessThanOrEqual : public RelationalOp
  {
    public:
      LessThanOrEqual() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("<=");
      };
  };

  class GreaterThan : public RelationalOp
  {
    public:
      GreaterThan() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print(">");
      };
  };

  class GreaterThanOrEqual : public RelationalOp
  {
    public:
      GreaterThanOrEqual() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print(">=");
      };
  };

  class Add : public AddOp
  {
    public:
      Add() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("+");
      };
  };

  class Subtract : public AddOp
  {
    public:
      Subtract() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("-");
      };
  };

  class Multiply : public MultOp
  {
    public:
      Multiply() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("*");
      };
  };

  class Divide : public MultOp
  {
    public:
      Divide() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("/");
      };
  };

  class Modulo : public MultOp
  {
    public:
      Modulo() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("%");
      };
  };

  class Negate : public UnaryOp
  {
    public:
      Negate() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("!");
      };
  };

  class Minus : public UnaryOp
  {
    public:
      Minus() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("-");
      };
  };

  class MethodInvocation : public UnaryOp
  {
    public:
      std::unique_ptr<IdentifierToken> ID;
      std::vector<std::unique_ptr<Expression>> expressions;
      
      MethodInvocation(std::unique_ptr<IdentifierToken> &ID, std::vector<std::unique_ptr<Expression>> &expressions) :
                         ID(std::move(ID)), expressions(std::move(expressions)) { };
        
      void toString(PrettyPrinter &printer) const {
        printer.print("." + ID->getIdentifier() + "(");
        
        bool continous = false;
        for(auto const& expression:expressions) {
          if(continous) {
            printer.print(", ");
          }
          expression->toString(printer);
          continous = true;
        }
        
        printer.print(")");
      };
  };

  class FieldAccess : public UnaryOp
  {
    public:
      std::unique_ptr<IdentifierToken> ID;
      
      FieldAccess(std::unique_ptr<IdentifierToken> &ID) : ID(std::move(ID)) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("." + ID->getIdentifier());
      };
  };

  class ArrayAccess : public UnaryOp
  {
    public:
      std::unique_ptr<Expression> expression;
      
      ArrayAccess(std::unique_ptr<Expression> &expression) : expression(std::move(expression)) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("[");
        expression->toString(printer);
        printer.print("]");
      };
  };
  
  
/*********************** Expressions ***********************/
  
  class OpExpression : public Expression
  {
    protected:
      OpExpression(std::unique_ptr<Op> op, std::unique_ptr<Expression> expression1,
                   std::unique_ptr<Expression> expression2) :
                     op(std::move(op)), expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    public:
      std::unique_ptr<Op> op;
      std::unique_ptr<Expression> expression1;
      std::unique_ptr<Expression> expression2;
      
      
      void toString(PrettyPrinter &printer) const {
        expression1->toString(printer);
        op->toString(printer);
        expression2->toString(printer);
      };
  };
  
  class AssignmentExpression : public Expression
  {
    public:
      std::unique_ptr<Expression> expression1;
      std::unique_ptr<Expression> expression2;
      
      AssignmentExpression(std::unique_ptr<Expression> &expression1, std::unique_ptr<Expression> &expression2) :
                             expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void toString(PrettyPrinter &printer) const {
        expression1->toString(printer);
        printer.print(" = ");
        expression2->toString(printer);
      };
  };
  
  class LogicalOrExpression : public Expression
  {
    public:
      std::unique_ptr<Expression> expression1;
      std::unique_ptr<Expression> expression2;
      
      LogicalOrExpression(std::unique_ptr<Expression> &expression1, std::unique_ptr<Expression> &expression2) :
                            expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void toString(PrettyPrinter &printer) const {
        expression1->toString(printer);
        printer.print(" || ");
        expression2->toString(printer);
      };
  };
  
  class LogicalAndExpression : public Expression
  {
    public:
      std::unique_ptr<Expression> expression1;
      std::unique_ptr<Expression> expression2;
      
      LogicalAndExpression(std::unique_ptr<Expression> &expression1, std::unique_ptr<Expression> &expression2) :
                             expression1(std::move(expression1)), expression2(std::move(expression2)) { };
    
      void toString(PrettyPrinter &printer) const {
        expression1->toString(printer);
        printer.print(" && ");
        expression2->toString(printer);
      };
  };
  
  class EqualityExpression : public OpExpression
  {
    public:
      EqualityExpression(std::unique_ptr<EqualityOp> &op, std::unique_ptr<Expression> &expression1,
                         std::unique_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
  };

  class RelationalExpression : public OpExpression
  {
    public:
      RelationalExpression(std::unique_ptr<RelationalOp> &op, std::unique_ptr<Expression> &expression1,
                           std::unique_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
  };
  
  class AdditiveExpression : public OpExpression
  {
    public:
      AdditiveExpression(std::unique_ptr<AddOp> &op, std::unique_ptr<Expression> &expression1,
                         std::unique_ptr<Expression> &expression2) : OpExpression(std::move(op), std::move(expression1), std::move(expression2)){ };
  };
  
  class MultiplicativeExpression : public OpExpression
  {
    public:
      MultiplicativeExpression(std::unique_ptr<MultOp> &op, std::unique_ptr<Expression> &expression1,
                               std::unique_ptr<Expression> &expression2) : 
                                 OpExpression(std::move(op), std::move(expression1), std::move(expression2)) { };
  };
  
  class CallExpression : public Expression
  {
    public:
      std::unique_ptr<IdentifierToken> ID;
      std::vector<std::unique_ptr<Expression>> expressions;
      
      CallExpression(std::unique_ptr<IdentifierToken> &ID, std::vector<std::unique_ptr<Expression>> &expressions) :
                       ID(std::move(ID)), expressions(std::move(expressions)) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print(ID->getIdentifier() + "(");
        
        bool continous = false;
        for(auto const& expression:expressions) {
          if(continous) {
            printer.print(", ");
          }
          expression->toString(printer);
          continous = true;
        }
        
        printer.print(")");
      };
  };
  
  class UnaryExpression : public Expression
  {
    public:
      std::unique_ptr<UnaryOp> op;
      std::unique_ptr<Expression> expression;
      
      UnaryExpression(std::unique_ptr<UnaryOp> &op, std::unique_ptr<Expression> &expression) :
                        op(std::move(op)), expression(std::move(expression)) { };
    
      void toString(PrettyPrinter &printer) const {
        op->toString(printer);
        expression->toString(printer);
      };
  };
  
  class CNull : public Expression
  {
    public:
      CNull() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("null");
      };
  };
  
  class CFalse : public Expression
  {
    public:
      CFalse() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("false");
      };
  };
  
  class CTrue : public Expression
  {
    public:
      CTrue() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("true");
      };
  };
  
  class CThis : public Expression
  {
    public:
      CThis() { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("this");
      };
  };
  
  class CIntegerLiteral : public Expression
  {
    public:
      std::unique_ptr<std::string> integer;
      
      CIntegerLiteral(std::unique_ptr<std::string> &integer) : integer(std::move(integer)) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print(*integer);
      };
  };
  
  class CRef : public Expression
  {
    public:
      std::unique_ptr<IdentifierToken> ID;
      
      CRef(std::unique_ptr<IdentifierToken> &ID) : ID(std::move(ID)) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print(ID->getIdentifier());
      };
  };
  
  class NewObject : public Expression
  {
    public:
      std::unique_ptr<IdentifierToken> ID;
      
      NewObject(std::unique_ptr<IdentifierToken> &ID) : ID(std::move(ID)) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("new " + ID->getIdentifier() + "()");
      };
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
    
      void toString(PrettyPrinter &printer) const {
        printer.print("new ");
        type->toString(printer);
        printer.print("[");
        expression->toString(printer);
        printer.print("]");
        
        for(int i=0; i<arrayDepth; i++) {
          printer.print("[]");
        }
      };
  };
  
  class Parameter : public Node
  {
    public:
      std::unique_ptr<Type>            type;
      std::unique_ptr<IdentifierToken> ID;
      
      Parameter(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierToken> &ID) :
            type(std::move(type)), ID(std::move(ID)) { };
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        printer.println(" " + ID->getIdentifier());
      };
  };
  
  
/*********************** Statements ***********************/
  
  
  class Block : public Statement
  {
    public:
      std::vector<std::unique_ptr<Statement>> statements;
      
      Block(std::vector<std::unique_ptr<Statement>> &statements) : statements(std::move(statements)) { };
      
      void toString(PrettyPrinter &printer) const {
        printer.println("{");
        printer.addIndent();
        for(auto const& statement : statements) {
          statement->toString(printer);
        }
        printer.removeIndent();
        printer.println("}");
      };
  };
  
  class IfStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      std::unique_ptr<Statement>  ifStatement;
      
      IfStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &ifStatement) :
                    expression(std::move(expression)), ifStatement(std::move(ifStatement)) { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("if (");
        expression->toString(printer);
        
        if(dynamic_cast<Block*>(ifStatement.get())) {
          printer.print(") ");
          ifStatement->toString(printer);
        } else {
          printer.println(")");
          printer.addIndent();
          ifStatement->toString(printer);
          printer.removeIndent();
        }
      };
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
      
      void toString(PrettyPrinter &printer) const {
        printer.print("if (");
        expression->toString(printer);
        
        if(dynamic_cast<Block*>(ifStatement.get())) {
          printer.print(") ");
          ifStatement->toString(printer);
        } else {
          printer.println(")");
          printer.addIndent();
          ifStatement->toString(printer);
          printer.removeIndent();
        }
        
        if(dynamic_cast<Block*>(elseStatement.get())) {
          printer.print("else ");
          elseStatement->toString(printer);
        } else {
          printer.println("else");
          printer.addIndent();
          elseStatement->toString(printer);
          printer.removeIndent();
        }
      };
  };
  
  class ExpressionStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      
      ExpressionStatement(std::unique_ptr<Expression> &expression) :
                            expression(std::move(expression)) { };
      
      void toString(PrettyPrinter &printer) const {
        expression->toString(printer);
        printer.println(";");
      };
  };
  
  class WhileStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      std::unique_ptr<Statement>  statement;
      
      WhileStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &statement) :
                       expression(std::move(expression)), statement(std::move(statement)) { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("while (");
        expression->toString(printer);
        if(dynamic_cast<Block*>(statement.get())) {
          printer.print(") ");
          statement->toString(printer);
        } else {
          printer.println(") ");
          printer.addIndent();
          statement->toString(printer);
          printer.removeIndent();
        }
      };
  };
  
  class ReturnExpressionStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression; 
      
      ReturnExpressionStatement(std::unique_ptr<Expression> &expression) :
                                  expression(std::move(expression)) { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("return ");
        expression->toString(printer);
        printer.println(";");
      };
  };
  
  class ReturnStatement : public Statement
  {
    public:
      ReturnStatement() { };
      
      void toString(PrettyPrinter &printer) const {
        printer.println("return;");
      };
  };
  
  class EmptyStatement : public Statement
  {
    public:
      EmptyStatement() { };
      
      void toString(PrettyPrinter &printer) const {
        printer.println(";");
      };
  };
  
  class LocalVariableDeclaration : public Statement
  {
    public:
      std::unique_ptr<Type>            type;
      std::unique_ptr<IdentifierToken> ID;
      
      LocalVariableDeclaration(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierToken> &ID) :
                                 type(std::move(type)), ID(std::move(ID)) { };
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        printer.println(" " + ID->getIdentifier() + ";");
      };
  };
  
  class LocalVariableExpressionDeclaration : public Statement
  {
    public:
      std::unique_ptr<Type>            type;
      std::unique_ptr<IdentifierToken> ID;
      std::unique_ptr<Expression>      expression;
      
      LocalVariableExpressionDeclaration(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierToken> &ID,
                                         std::unique_ptr<Expression> &expression) :
                                           type(std::move(type)), ID(std::move(ID)), expression(std::move(expression)) { };
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        printer.println(" " + ID->getIdentifier());
        printer.println(" = ");
        expression->toString(printer);
        printer.println(";");
      };
  };
  
  
/*********************** Class related ***********************/
  
  
  class Field : public ClassMember
  {
    public:
      std::unique_ptr<Type> type;
      std::unique_ptr<IdentifierToken> ID;
      
      Field(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierToken> &ID) :
            type(std::move(type)), ID(std::move(ID)) { };
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        printer.println(" " + ID->getIdentifier() + ";");
      };
  };
  
  class Method : public ClassMember
  {
    public:
      std::unique_ptr<Type> type;
      std::unique_ptr<IdentifierToken> ID;
      std::vector<std::unique_ptr<Parameter>> parameters;
      std::unique_ptr<Block> block;
      
      Method(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierToken> &ID,
             std::vector<std::unique_ptr<Parameter>> &parameters, std::unique_ptr<Block> &block) :
               type(std::move(type)), ID(std::move(ID)), parameters(std::move(parameters)), block(std::move(block)) { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("public ");
        type->toString(printer);
        printer.print(ID->getIdentifier() + "(");

        bool continous = false;
        for(auto const& parameter:parameters) {
          if(continous) {
            printer.print(", ");
          }
          parameter->toString(printer);
          continous = true;
        }

        printer.print(")");
        block->toString(printer);
      };
  };
  
  /** The MainMethod is a ClassMember and therefore part of a ClassDeclaration. The MainMethod element contains two IDs (Method-identifier and parameter-identifier) and a Block (Representation of the method body) */
  class MainMethod : public ClassMember
  {
    public:
      std::unique_ptr<IdentifierToken> ID;
      std::unique_ptr<IdentifierToken> parameterID;
      std::unique_ptr<Block>             block;
      
      MainMethod(std::unique_ptr<IdentifierToken> &ID, std::unique_ptr<IdentifierToken> &parameterID,
                 std::unique_ptr<Block> &block) :
                   ID(std::move(ID)), parameterID(std::move(parameterID)), block(std::move(block)) { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("public static void " + ID->getIdentifier() + "(String[] " + parameterID->getIdentifier() + ")");
        block->toString(printer);
      };
  };
  
  /** A ClassDeclaration consists of an ID, which is the identifier for this class and various ClassMembers, like Methods and Fields.  */
  class ClassDeclaration : public Node
  {
    public:
      std::unique_ptr<IdentifierToken> ID;
      std::vector<std::unique_ptr<ClassMember>> classMembers;
      
      ClassDeclaration(std::unique_ptr<IdentifierToken> &ID,
                       std::vector<std::unique_ptr<ClassMember>> &classMembers) :
                         ID(std::move(ID)), classMembers(std::move(classMembers)) { };
      
      void toString(PrettyPrinter &printer) const {
        // sort the classMembers
        // first methods then fields, each of them in alphabetical order
        std::sort(classMembers.begin(), classMembers.end(), 
          [](const std::unique_ptr<ClassMember> &a, const std::unique_ptr<ClassMember> &b) -> bool{
            Field* a_p;
            Field* b_p;
            
            if ((a_p = dynamic_cast<Field*>(a.get())) && (b_p = dynamic_cast<Field*>(b.get())))
            {
              // a and b are both Fields
              
              // get identifier strings and compare them
              return a_p->ID.get()->getIdentifier().compare(b_p->ID.get()->getIdentifier());
            }
            else if ((dynamic_cast<Method*>(a.get()) || dynamic_cast<MainMethod*>(a.get())) && 
              (dynamic_cast<Method*>(b.get()) || dynamic_cast<MainMethod*>(b.get())))
            {
              // a and b are both either Method or MainMethod
              std::string a_s;
              std::string b_s;
              Method* m;
              MainMethod* mm;
              
              // get identifier string for a
              if (m = dynamic_cast<Method*>(a.get()))
              {
                a_s = m->ID.get()->getIdentifier();
              }
              else if (mm = dynamic_cast<MainMethod*>(a.get()))
              {
                a_s = mm->ID.get()->getIdentifier();
              }
              
              // get identifier string for b
              if (m = dynamic_cast<Method*>(b.get()))
              {
                b_s = m->ID.get()->getIdentifier();
              }
              else if (mm = dynamic_cast<MainMethod*>(b.get()))
              {
                b_s = mm->ID.get()->getIdentifier();
              }
              
              // compare strings
              return a_s.compare(b_s);
            }
            else if (dynamic_cast<Field*>(a.get()) && 
              (dynamic_cast<Method*>(b.get()) || dynamic_cast<MainMethod*>(b.get())))
            {
              // a is field and b is Method or MainMethod
              return 1;
            }
            else
            {
              // a is Method or MainMethod and b is Field
              return -1;
            }
        });
        
        printer.println("class "+ ID->getIdentifier() + " {");
        printer.addIndent();
        for(auto const& classMember : classMembers) {
          classMember->toString(printer);
        }
        printer.removeIndent();
        printer.println("}");
      };
  };
  
  /** Always the first element in the AST. Can contain multiple ClassDeclarations. */	
  class Program : public Node
  {
    public:
      std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;
      
      Program(std::vector<std::unique_ptr<ClassDeclaration>> &classDeclarations) :
                classDeclarations(std::move(classDeclarations)) { };

      void toString(PrettyPrinter &printer) const {
        for(auto const& classDeclaration : classDeclarations) {
          classDeclaration->toString(printer);
        }
      };
  };
}
