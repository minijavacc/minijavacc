#pragma once

#include "prettyprinter.h"
#include "../stringtable/stringtable.h"

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

  class BasicType    : public Node { };
  class ClassMember  : public Node { };
  class Expression   : public Node { };
  class Statement    : public Node { };

  class EqualityOp   : public Node { };
  class RelationalOp : public Node { };
  class AddOp        : public Node { };
  class MultOp       : public Node { };
  class UnaryOp      : public Node { };
  
  class UserType : public BaseType
  {
    
  }
  
  //TODO
  class Type : public Node
  {
    std::unique_ptr<BasicType> basicType;
    int
  }
  
  class TypeInt : public BasicType
  {
    TypeInt() { };
    
    void toString(PrettyPrinter &printer) {
      printer.print("int");
    }
  }
  
  class TypeBoolean : public BasicType
  {
    TypeBoolean() { };
    
    void toString(PrettyPrinter &printer) {
      printer.print("boolean");
    }
  }
  
  class TypeVoid : public BasicType
  {
    void toString(PrettyPrinter &printer) {
      printer.print("void");
    }
  }
  
  class UserType : public BasicType
  {
    std::unique_ptr<IdentifierTokenId> ID;
    
    UserType(std::unique_ptr<IdentifierTokenId> &ID) : ID(std::move(ID)) { }
    
    void toString(PrettyPrinter &printer) {
      printer.print("" + ID); //TODO
    }

  }

  class NotEquals : public EqualityOp
  {
    NotEquals() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class Equals : public EqualityOp
  {
    Equals() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class LessThan : public RelationalOp
  {
    LessThan() { }
  }

  class LessThanOrEqual : public RelationalOp
  {
    LessThanOrEqual() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class GreaterThan : public RelationalOp
  {
    GreaterThan() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class GreaterThanOrEqual : public RelationalOp
  {
    GreaterThanOrEqual() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class Add : public AddOp
  {
    Add() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class Subtract : public AddOp
  {
    Subtract() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class Multiply : public MultOp
  {
    Multiply() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class Divide : public MultOp
  {
    Divide() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class Modulo : public MultOp
  {
    Modulo() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class Negate : public UnaryOp
  {
    Negate() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class Minus : public UnaryOp
  {
    Minus() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class MethodInvocation : public UnaryOp
  {
    std::unique_ptr<IdentifierTokenId> ID;
    std::vector<std::unique_ptr<Expression>> expression;
    
    MethodInvocation()

  }

  class FieldAccess : public UnaryOp
  {
    FieldAccess() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }

  class ArrayAccess : public UnaryOp
  {
    ArrayAccess() { };
  
    void toString(PrettyPrinter &printer) {
      printer.print("");
    }
  }
  
MethodInvocation :: Ident [Expression]

FieldAccess :: Ident

ArrayAccess :: Expression

Expression = AssignmentExpression 
           | LogicalOrExpression 
           | LogicalAndExpression 
           | EqualityExpression
           | RelationalExpression
           | AdditiveExpression 
           | MultiplicativeExpression
           | CallExpression
           | UnaryExpression
           | CNull
           | CFalse
           | CTrue
           | CIntegerLiteral
           | CRef
           | CThis
           | NewObject
           | NewArray
           
AssignmentExpression :: Expression Expression
LogicalOrExpression :: Expression Expression
LogicalAndExpression :: Expression Expression
EqualityExpression :: EqualityOp Expression Expression
RelationalExpression :: RelationalOp Expression Expression
AdditiveExpression :: AddOp Expression Expression
MultiplicativeExpression :: MultOp Expression Expression
CallExpression :: Ident [Expression]
UnaryExpression :: UnaryOp Expression
CIntegerLiteral :: Int
CRef :: Ident
NewObject :: Ident
NewArray :: Int BasicType Expression
  class Type : public Node
  {
    public:
      int         arrayDepth;
      
      std::unique_ptr<BasicType> basicType; //TODO
      
      Type(int &arrayDepth, std::string &type) :
           arrayDepth(arrayDepth), type(type) { };
      
      void toString(PrettyPrinter &printer) {
        basicType.toString(printer);
        for(int i=0;i<arrayDepth;i++) {
          printer.print("[]");
        }
      };
  };
  
  class Parameter : public Node
  {
    public:
      std::unique_ptr<Type>              type;
      std::unique_ptr<IdentifierTokenId> ID;
      
      Parameter(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierTokenId> &ID) :
            type(type), ID(std::move(ID)) { };
      
      void toString(PrettyPrinter &printer) {
        type.toString(printer);
        printer.println(" " + ID);
      };
  };

  class Block : public Statement
  {
    public:
      std::vector<std::unique_ptr<Statement>> statements;
      
      Block(std::vector<std::unique_ptr<Statement>> &statements) :
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
      std::unique_ptr<Statement>  ifStatement;
      
      IfStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &ifStatement) :
                    expression(std::move(expression)), ifStatement(std::move(ifStatement)) { };
      
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
      std::unique_ptr<Statement>  ifStatement;
      std::unique_ptr<Statement>  elseStatement;
      
      IfElseStatement(std::unique_ptr<Expression> &expression, std::unique_ptr<Statement> &ifStatement,
                      std::unique_ptr<Statement> &elseStatement) :
                        expression(std::move(expression)), ifStatement(std::move(ifStatement)),
                        elseStatement(std::move(elseStatement)) { };
      
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
  
  class ExpressionStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression;
      
      ExpressionStatement(std::unique_ptr<Expression> &expression) :
                     expression(std::move(expression)) { };
      
      void toString(PrettyPrinter &printer) {
        expression.toString(printer);
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
  
  class ReturnExpressionStatement : public Statement
  {
    public:
      std::unique_ptr<Expression> expression; 
      
      ReturnExpressionStatement(std::unique_ptr<Expression> &expression) :
                                  expression(std::move(expression)) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("return ");
        expression.toString(printer);
        printer.println(";");
      };
  };
  
  class ReturnStatement : public Statement
  {
    public:
      ReturnStatement() { };
      
      void toString(PrettyPrinter &printer) {
        printer.println("return;");
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
  
  class LocalVariableDeclaration : public Statement
  {
    public:
      std::unique_ptr<Type>              type;
      std::unique_ptr<IdentifierTokenId> ID;
      
      LocalVariableDeclaration(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierTokenId> &ID) :
                                 type(std::move(type)), ID(std::move(ID)) { };
      
      void toString(PrettyPrinter &printer) {
        type.print(printer);
        printer.println(ID + ";"); //TODO
      };
  }
  
  class LocalVariableExpressionDeclaration : public Statement
  {
    public:
      std::unique_ptr<Type>              type;
      std::unique_ptr<IdentifierTokenId> ID;
      std::unique_ptr<Expression>        expression;
      
      LocalVariableExpressionDeclaration(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierTokenId> &ID,
                                         std::unique_ptr<Expression> &expression) :
                                           type(std::move(type)), ID(std::move(ID)), expression(std::move(expression))
        { };
      
      void toString(PrettyPrinter &printer) {
        type.print(printer);
        printer.println(ID + ";"); //TODO
      };
  }
  
  class Field : public ClassMember
  {
    public:
      std::unique_ptr<IdentifierTokenId> ID;
      std::unique_ptr<Type> type;
      
      Field(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierTokenId> &ID) :
            type(std::move(type)), ID(std::move(ID)) { };
      
      void toString(PrettyPrinter &printer) {
        type.toString(printer);
        printer.println(" " + ID + ";");
      };
  };
  
  class Method : public ClassMember
  {
    public:
      std::unique_ptr<Type> type;
      std::unique_ptr<IdentifierTokenId> ID;
      std::vector<std::unique_ptr<Parameter>> parameters;
      std::unique_ptr<Block> block;
      
      Method(std::unique_ptr<Type> &type, std::unique_ptr<IdentifierTokenId> &ID, std::vector<std::unique_ptr<Parameter>> &parameters,
             std::unique_ptr<Block> &block) :
               type(std::move(type)), ID(std::move(ID)), parameters(std::move(parameters)), block(std::move(block)) { };
      
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
      std::unique_ptr<IdentifierTokenId> ID;
      std::unique_ptr<IdentifierTokenId> parameterID;
      std::unique_ptr<Statement> statement;
      
      MainMethod(std::unique_ptr<IdentifierTokenId> &ID, std::unique_ptr<Statement> &statement,
                 std::unique_ptr<IdentifierTokenId> &parameterID) :
                   ID(std::move(ID)), statement(std::move(statement)), parameterID(std::move(parameterID)) { };
      
      void toString(PrettyPrinter &printer) {
        printer.print("public static void " + ID +"(String[] " + parameterID + ")"); //TODO
        block.toString(printer);
      };
  };
  
  class ClassDeclaration : public Node
  {
    public:
      std::unique_ptr<IdentifierTokenId> ID;
      std::vector<std::unique_ptr<ClassMember>> classMembers;
      
      Class(std::unique_ptr<IdentifierTokenId> &ID, std::vector<std::unique_ptr<ClassMember>> &classMembers) :
            ID(std::move(ID)), classMembers(classMembers) { };
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
      std::vector<std::unique_ptr<ClassDeclaration>> classDeclarations;
      
      Program(std::vector<std::unique_ptr<ClassDeclaration>> &classDeclarations) :
              classDeclarations(classDeclarations) { };

      void toString(PrettyPrinter &printer) {
        for(auto const& classDeclaration : classDeclarations) {
          classDeclaration.toString(printer);
        }
      };
  };
}
