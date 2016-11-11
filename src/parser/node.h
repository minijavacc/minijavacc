#pragma once

#include "prettyprinter.h"

#include <memory>
#include <string>
#include <vector>

namespace cmpl
{
  // abstract classes for categorization
  class Node
  {
    public:
      virtual void toString(PrettyPrinter printer) const = 0; // must be implemented by subclasses
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
      
      Type(int &arrayDepth, std::string &type) :
           arrayDepth(arrayDepth), type(type) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print(type);
        for(int i=0;i<arrayDepth;i++) {
          printer.print("[]");
        }
      };
  };
  
  class Parameter : public Node
  {
    public:
      std::unique_ptr<Type> type;
      IdentifierTokenId ID;
      
      Parameter(std::unique_ptr<Type> &type, IdentifierTokenId &ID) :
            type(type), ID(ID) { };
      
      void toString(PrettyPrinter &printer) {
        type.toString(printer);
        printer.println(" " + ID);
      };
  };

  class Block : public Node
  {
    public:
      std::vector<std::unique_ptr<BlockStatement>> statements;
      
      Block(std::vector<std::unique_ptr<BlockStatement>> &statements) :
            statements(statements) { };
      
      void toString(PrettyPrinter &printer) {
        printer.println("{");
        printer.addIndent();
        for(auto const& statement : statements) {
          statement.toString(printer);
        }
        printer.removeIndent();
        printer.println("}");
      };
  };
  
  
  class IfStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      Statement ifStatement;
      
      IfStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &ifStatement) :
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
      std::unique_ptr<Expression> expression;
      std::unique_ptr<Statement> ifStatement;
      std::unique_ptr<Statement> elseStatement;
      
      IfElseStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &ifStatement,
                      std::unique_ptr<Statement> &elseStatement) :
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
      std::unique_ptr<Expression> expression;
      std::unique_ptr<Statement> statement;
      
      WhileStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &statement) :
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
      std::unique_ptr<Expression> expression;
      
      ExpressionStatement(std::unique_ptr<Expression> &expression) :
                     expression(expression) { };
      
      void toString(PrettyPrinter &printer) {
        expression.toString(printer);
        printer.println(";");
      };
  };
  
  class LocalVarDeclStatement : public BlockStatement
  {
    public:
      std::unique_ptr<Type> type;
      IdentifierTokenId ID;
      LocalVarDeclStatement(std::unique_ptr<Type> &type, IdentifierTokenId &ID) :
           type(type), ID(ID) { };
      
      void toString(PrettyPrinter &printer) {
        type.print(printer);
        printer.println(ID + ";");
      };
  }
  
  class Field : public ClassMember
  {
    public:
      std::unique_ptr<Type> type;
      IdentifierTokenId ID;
      
      Field(std::unique_ptr<Type> &type, IdentifierTokenId &ID) :
            type(type), ID(ID) { };
      
      void toString(PrettyPrinter &printer) {
        type.toString(printer);
        printer.println(" " + ID + ";");
      };
  };
  
  class Method : public ClassMember
  {
    public:
      std::unique_ptr<Type> type;
      IdentifierTokenId ID;
      std::vector<std::unique_ptr<Parameter>> parameters;
      std::unique_ptr<Block> block;
      
      Method(std::unique_ptr<Type> &type, IdentifierTokenId &ID, std::vector<std::unique_ptr<Parameter>> &parameters,
             std::unique_ptr<Block> &block) :
               type(type), ID(ID), parameters(parameters), block(block) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("public ");
        type.toString(printer);
        printer.print(" " + ID + "(");
        if(parameters.size() >= 1) {
          parameters[0].toString(printer);
        }
        for(int i=1; i < parameters.size(); i++) {
          printer.print(", ");
          parameters[i].toString(printer);
        }
        printer.print(")");
        block.toString(printer);
      };
  };
  
  class MainMethod : public ClassMember
  {
    public:
      IdentifierTokenId ID;
      std::string parameterID;
      std::unique_ptr<Block> block;
      
      MainMethod(std::unique_ptr<Type> &type, IdentifierTokenId &ID, std::unique_ptr<Block> &block,
                 std::string &parameterID) :
                   type(type), ID(ID), block(block), parameterID(parameterID) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("public static void " + ID +"(String[] " + parameterID + ")");
        block.toString(printer);
      };
  };
  
  class Class : public Node
  {
    public:
      IdentifierTokenId ID;
      std::vector<std::unique_ptr<ClassMember>> classMembers;
      
      Class(IdentifierTokenId &ID, std::vector<std::unique_ptr<ClassMember>> &classMembers) :
            ID(ID), classMembers(classMembers) { };
      void toString(PrettyPrinter &printer) {
        printer.println("class " + ID + "{");
        printer.addIndent();
        for(auto const& classMember : classMembers) {
          classMember.toString(printer);
        }
        printer.removeIndent();
        printer.println("}");
      };
  };
  
  class Program : public Node
  {
    public:
      std::vector<std::unique_ptr<Class>> classes;
      
      Program(std::vector<std::unique_ptr<Class>> &classes) :
              classes(classes) { };

      void toString(PrettyPrinter &printer) {
        for(auto const& clss : classes) {
          clss.toString(printer);
        }
      };
  };
}
