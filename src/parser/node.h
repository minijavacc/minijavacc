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

  class BasicType      : public Node           { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class ClassMember    : public Node           { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class Expression     : public Node           { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class BlockStatement : public Node           { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class Statement      : public BlockStatement { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class Op             : public Node           { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class EqualityOp     : public Op             { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class RelationalOp   : public Op             { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class AddOp          : public Op             { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class MultOp         : public Op             { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  class UnaryOp        : public Op             { public: virtual void toString(PrettyPrinter &printer) const = 0; };
  
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
      StringIdentifier ID;
      
      UserType(StringIdentifier &ID) : ID(ID) { }
      
      void toString(PrettyPrinter &printer) const {
        printer.print(StringTable::lookupIdentifier(ID));
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
      StringIdentifier                         ID;
      std::vector<std::unique_ptr<Expression>> arguments;
      
      MethodInvocation(StringIdentifier &ID, std::vector<std::unique_ptr<Expression>> &arguments) :
                         ID(ID), arguments(std::move(arguments)) { };
        
      void toString(PrettyPrinter &printer) const {
        printer.print("." + StringTable::lookupIdentifier(ID) + "(");
        
        bool continous = false;
        for(auto const& argument:arguments) {
          if(continous) {
            printer.print(", ");
          }
          argument->toString(printer);
          continous = true;
        }
        
        printer.print(")");
      };
  };

  class FieldAccess : public UnaryOp
  {
    public:
      StringIdentifier ID;
      
      FieldAccess(StringIdentifier &ID) : ID(ID) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("." + StringTable::lookupIdentifier(ID));
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
      std::unique_ptr<Op>         op;
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
      StringIdentifier                         ID;
      std::vector<std::unique_ptr<Expression>> arguments;
      
      CallExpression(StringIdentifier &ID, std::vector<std::unique_ptr<Expression>> &arguments) :
                       ID(ID), arguments(std::move(arguments)) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print(StringTable::lookupIdentifier(ID) + "(");
        
        bool continous = false;
        for(auto const& argument:arguments) {
          if(continous) {
            printer.print(", ");
          }
          argument->toString(printer);
          continous = true;
        }
        
        printer.print(")");
      };
  };
  
  class UnaryLeftExpression : public Expression
  {
    public:
      std::unique_ptr<UnaryOp>    op;
      std::unique_ptr<Expression> expression;
      
      UnaryLeftExpression(std::unique_ptr<UnaryOp> &op, std::unique_ptr<Expression> &expression) :
                            op(std::move(op)), expression(std::move(expression)) { };
    
      void toString(PrettyPrinter &printer) const {
        op->toString(printer);
        expression->toString(printer);
      };
  };
  
  class UnaryRightExpression : public Expression
  {
    public:
      std::unique_ptr<UnaryOp>    op;
      std::unique_ptr<Expression> expression;
      
      UnaryRightExpression(std::unique_ptr<Expression> &expression, std::unique_ptr<UnaryOp> &op) :
                             expression(std::move(expression)), op(std::move(op)) { };
    
      void toString(PrettyPrinter &printer) const {
        expression->toString(printer);
        op->toString(printer);
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
      std::string integer;
      
      CIntegerLiteral(std::string &integer) : integer(integer) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print(integer);
      };
  };
  
  class CRef : public Expression
  {
    public:
      StringIdentifier ID;
      
      CRef(StringIdentifier &ID) : ID(ID) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print(StringTable::lookupIdentifier(ID));
      };
  };
  
  class NewObject : public Expression
  {
    public:
      StringIdentifier ID;
      
      NewObject(StringIdentifier &ID) : ID(ID) { };
    
      void toString(PrettyPrinter &printer) const {
        printer.print("new " + StringTable::lookupIdentifier(ID) + "()");
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
      std::unique_ptr<Type> type;
      StringIdentifier      ID;
      
      Parameter(std::unique_ptr<Type> &type, StringIdentifier &ID) :
            type(std::move(type)), ID(ID) { };
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        printer.println(" " + StringTable::lookupIdentifier(ID));
      };
  };
  
  
/*********************** Statements ***********************/
  
  
  class Block : public Statement
  {
    public:
      std::vector<std::unique_ptr<BlockStatement>> statements;
      
      Block(std::vector<std::unique_ptr<BlockStatement>> &statements) : statements(std::move(statements)) { };
      
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
  
  class LocalVariableDeclaration : public BlockStatement
  {
    public:
      std::unique_ptr<Type> type;
      StringIdentifier      ID;
      
      LocalVariableDeclaration(std::unique_ptr<Type> &type, StringIdentifier &ID) :
                                 type(std::move(type)), ID(ID) { };
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        printer.println(" " + StringTable::lookupIdentifier(ID) + ";");
      };
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
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        printer.println(" " + StringTable::lookupIdentifier(ID));
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
      StringIdentifier      ID;
      
      Field(std::unique_ptr<Type> &type, StringIdentifier &ID) :
            type(std::move(type)), ID(ID) { };
      
      void toString(PrettyPrinter &printer) const {
        type->toString(printer);
        printer.println(" " + StringTable::lookupIdentifier(ID) + ";");
      };
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
      
      void toString(PrettyPrinter &printer) const {
        printer.print("public ");
        type->toString(printer);
        printer.print(" ");
        printer.print(StringTable::lookupIdentifier(ID) + "(");

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
      StringIdentifier       ID;
      StringIdentifier       parameterID;
      std::unique_ptr<Block> block;
      
      MainMethod(StringIdentifier &ID, StringIdentifier &parameterID, std::unique_ptr<Block> &block) :
                   ID(ID), parameterID(std::move(parameterID)), block(std::move(block)) { };
      
      void toString(PrettyPrinter &printer) const {
        printer.print("public static void " + StringTable::lookupIdentifier(ID) + "(String[] " + StringTable::lookupIdentifier(parameterID) + ")");
        block->toString(printer);
      };
  };
  
  /** A ClassDeclaration consists of an ID, which is the identifier for this class and various ClassMembers, like Methods and Fields.  */
  class ClassDeclaration : public Node
  {
    public:
      StringIdentifier                          ID;
      std::vector<std::unique_ptr<ClassMember>> classMembers;
      
      ClassDeclaration(StringIdentifier &ID, std::vector<std::unique_ptr<ClassMember>> &classMembers) :
                         ID(ID), classMembers(std::move(classMembers)) { };
      
      void toString(PrettyPrinter &printer) const {
        // sort the classMembers
        // first methods then fields, each of them in alphabetical order
        
        // TODO: sort() throws compiler error
        /*std::sort(classMembers.begin(), classMembers.end(), 
          [](const std::unique_ptr<ClassMember> &a, const std::unique_ptr<ClassMember> &b) -> bool{
            Field* a_p;
            Field* b_p;
            
            if ((a_p = dynamic_cast<Field*>(a.get())) && (b_p = dynamic_cast<Field*>(b.get())))
            {
              // a and b are both Fields
              
              // get identifier strings and compare them
              return StringTable::lookupIdentifier(ap_ID).compare(StringTable::lookupIdentifier(b_pID));
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
                a_s = StringTable::lookupIdentifier(m->ID);
              }
              else if (mm = dynamic_cast<MainMethod*>(a.get()))
              {
                a_s = StringTable::lookupIdentifier(mm->ID);
              }
              
              // get identifier string for b
              if (m = dynamic_cast<Method*>(b.get()))
              {
                b_s = StringTable::lookupIdentifier(m->ID);
              }
              else if (mm = dynamic_cast<MainMethod*>(b.get()))
              {
                b_s = StringTable::lookupIdentifier(mm->ID);
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
        });*/
        
        printer.println("class "+ StringTable::lookupIdentifier(ID) + " {");
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
        // sort classes by alphabetical order
        
        // TODO: sort() throws compiler error
        /*std::sort(classDeclarations.begin(), classDeclarations.end(), 
          [](const std::unique_ptr<ClassDeclaration> &a, const std::unique_ptr<ClassDeclaration> &b) -> bool{
            return StringTable::lookupIdentifier(a->ID).compare(StringTable::lookupIdentifier(b->ID));
        });*/
        
        for(auto const& classDeclaration : classDeclarations) {
          classDeclaration->toString(printer);
        }
      };
  };
}
