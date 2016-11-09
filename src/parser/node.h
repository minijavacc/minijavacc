#pragma once

#include "prettyprinter.h"

#include <memory>
#include <vector>
#include <string>

namespace cmpl
{
  // abstract classes for categorization
  class Node
  {
    public:
      virtual void toString(PrettyPrinter printer) = 0; // must be implemented by subclasses
  };
  
/**************** actual nodes ****************/

  class Expression : public Node { };
  class BlockStatement : public Node { };
  class Statement : public BlockStatement { };
  class ClassMember : public Node { };
  
  class Type : public Node
  {
    public:
      int arrayDepth;
      std::string type;
      
      Type(int arrayDepth, std::string type) :
           arrayDepth(arrayDepth), type(type) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print(type);
        for(int i=0;i<arrayDepth;i++) {
          printer.print([]);
        }
      };
  };
  
  class Block : public Node
  {
    public:
      std::vector<BlockStatement> statements;
      
      Block(std::vector<BlockStatement> statements) :
            statements(statements) { };
      
      void toString(PrettyPrinter &printer) {
        printer.println("{");
        printer.addIndent();
        for(BlockStatement statement : statements) {
          statement.toString(printer);
        }
        printer.removeIndent();
        printer.println("}");
      };
  };
  
  
  class IfStatement : public Statement
  {
    public:
      Expression &expression;
      Statement &ifStatement;
      
      IfStatement(Expression &expression, Statement &ifStatement) :
                  expression(expression), ifStatement(ifStatement) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("if (");
        expression.toString(printer);
        
        if(dynamic_cast<Block>(ifStatement)) {
          printer.print(") ");
          ifStatement.toString(printer);
        } else {
          printer.println(")");
          printer.addIndent();
          statement.toString(printer);
          printer.removeIndent();
        }
      };
  };
  
  class IfElseStatement : public Statement
  {
    public:
      Expression expression;
      Statement ifStatement;
      Statement elseStatement;
      
      IfElseStatement(Expression expression, Statement ifStatement, Statement elseStatement) :
                      expression(expression), ifStatement(ifStatement), elseStatement(elseStatement) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("if (");
        expression.toString(printer);
        
        if(dynamic_cast<Block>(ifStatement)) {
          printer.print(") ");
          ifStatement.toString(printer);
        } else {
          printer.println(")");
          printer.addIndent();
          statement.toString(printer);
          printer.removeIndent();
        }
        
        if(dynamic_cast<Block>(elseStatement)) {
          printer.print("else ");
          elseStatement.toString(printer);
        } else {
          printer.println("else");
          printer.addIndent();
          statement.toString(printer);
          printer.removeIndent();
        }
      };
  };
  
  
  class EmptyStatement : public Statement
  {
    public:
      EmptyStatement() { };
      
      void toString(PrettyPrinter &printer) {
        printer.println(";");
      };
  };
  
  class WhileStatement : public Statement
  {
    public:
      Expression expression;
      Statement statement;
      
      WhileStatement(Expression expression, Statement statement) :
                     expression(expression), statement(statement) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("while (");
        expression.toString(printer);
        if(dynamic_cast<Block>(statement)) {
          printer.print(") ");
          statement.toString(printer);
        } else {
          printer.println(") ");
          printer.addIndent();
          statement.toString(printer);
          printer.removeIndent();
        }
      };
  };
  
  class ExpressionStatement : public Statement
  {
    public:
      Expression expression;
      
      ExpressionStatement(Expression expression) :
                     expression(expression) { };
      
      void toString(PrettyPrinter &printer) {
        expression.toString(printer);
        printer.println(";");
      };
  };
  
  class LocalVarDeclStatement : public BlockStatement
  {
    public:
      Type type;
      std::string ID;
      LocalVarDeclStatement(Type type, std::string ID) :
           type(type), ID(ID) { };
      
      void toString(PrettyPrinter &printer) {
        type.print(printer);
        printer.println(ID + ";");
      };
  }
  
  class Field : public ClassMember
  {
    public:
      Type type;
      std:string ID;
      
      Field(Type type, std::string ID) :
            type(type), ID(ID) { };
      
      void toString(PrettyPrinter &printer) {
        type.toString(printer);
        printer.println(" " + ID + ";");
      };
  };
  
  class Method : public ClassMember
  {
    public:
      Type type;
      std:string ID;
      std::vector<Parameter> parameters;
      Block block;
      
      Method(Type type, std::string ID, std::vector<Parameter> parameters, Block block) :
             type(type), ID(ID), parameters(parameters), block(block) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("public ");
        type.toString(printer);
        printer.print(" " + ID + "(");
        if(params.size() >= 1) {
          parameters[0].toString(printer);
        }
        for(int i=1; i < params.size(); i++) {
          printer.print(", ");
          parameters[i].toString(printer);
        }
        printer.print(params);
        printer.print(")");
        block.toString(printer);
      };
  };
  
  class MainMethod : public ClassMember
  {
    public:
      std:string ID;
      std:string parameterID;
      Block block;
      
      MainMethod(Type type, std::string ID, Block block, std::string parameterID) :
                 type(type), ID(ID), block(block), parameterID(parameterID) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("public static void " + ID +"(String[] " + parameterID + ")");
        block.toString(printer);
      };
  };
  
  class Class : public Node
  {
    public:
      std::vector<ClassMember> classMembers;
      
      Class(std::vector<ClassMember> classMembers) : classMembers(classMembers) { };
      void toString(PrettyPrinter &printer) {
        printer.println("class " + ID + "{");
        printer.addIndent();
        for(auto Class : Classs) {
          Class.toString(printer);
        }
        printer.removeIndent();
        printer.println("}");
      };
  };
  
  class Program : public Node
  {
    public:
      std::string ID;
      std::vector<Class> classes;
      
      Program(std::string ID, std::vector<Class> classes) :
              ID(ID), classes(classes) { };

      void toString(PrettyPrinter &printer) {
        for(auto clss : classes) {
          clss.toString(printer);
        }
      };
  };

















  
}
