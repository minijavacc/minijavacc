#include "prettyprinter.h"

#include <iostream>

using namespace cmpl;

void PrettyPrinter::print(const std::string &s)
{
  if(indentsPrinted) {
    printStream << s;
  } else {
    printStream << indents << s;
    indentsPrinted = true;
  }
}

void PrettyPrinter::println(const std::string &s)
{
  printStream << indents << s << '\n';
  indentsPrinted = false;
}

void PrettyPrinter::addIndent()
{
  indents.append(indent);
}

void PrettyPrinter::removeIndent()
{
  indents.erase(indents.begin(),indents.end()-indent.size());
}

std::ostream& PrettyPrinter::getStream()
{
  return printStream;
}





void PrettyPrinter::dispatch(Program &n) {
  for(auto const& classDeclaration : n.classDeclarations) {
    classDeclaration->accept(*this);
  }
};

void PrettyPrinter::dispatch(ClassDeclaration &n) {
  println("class "+ StringTable::lookupIdentifier(n.ID) + " {");

  addIndent();
  
  for(auto const& classMember : n.classMembers) {
    classMember->accept(*this);
  }
  
  removeIndent();
  println("}");
};

void PrettyPrinter::dispatch(MainMethod &n) {
  print("public static void " + StringTable::lookupIdentifier(n.ID) + "(String[] " + StringTable::lookupIdentifier(n.parameterID) + ")");
  n.block->accept(*this);
};

void PrettyPrinter::dispatch(Field &n) {
  n.type->accept(*this);
  println(" " + StringTable::lookupIdentifier(n.ID) + ";");
};

void PrettyPrinter::dispatch(Method &n) {
  print("public ");
  n.type->accept(*this);
  print(" ");
  print(StringTable::lookupIdentifier(n.ID) + "(");
  
  bool continous = false;
  for(auto const& parameter:n.parameters) {
    if(continous) {
      print(", ");
    }
    parameter->accept(*this);
    continous = true;
  }
  
  print(")");
  n.block->accept(*this);
};

void PrettyPrinter::dispatch(Type &n) {
  n.type->accept(*this);
  for(int i=0; i<n.arrayDepth; i++) {
    print("[]");
  }
};

void PrettyPrinter::dispatch(UserType &n) {
  print(StringTable::lookupIdentifier(n.ID));
};

void PrettyPrinter::dispatch(TypeInt &n) {
  print("int");
};

void PrettyPrinter::dispatch(TypeBoolean &n) {
  print("boolean");
};

void PrettyPrinter::dispatch(TypeVoid &n) {
  print("void");
};

void PrettyPrinter::dispatch(Parameter &n) {
  n.type->accept(*this);
  println(" " + StringTable::lookupIdentifier(n.ID));
};

void PrettyPrinter::dispatch(Block &n) {
  println("{");
  addIndent();
  for(auto const& statement : n.statements) {
    statement->accept(*this);
  }
  removeIndent();
  println("}");
};

void PrettyPrinter::dispatch(IfStatement &n) {
  print("if (");
  n.expression->accept(*this);
  
  if(dynamic_cast<Block*>(n.ifStatement.get())) {
    print(") ");
    n.ifStatement->accept(*this);
  } else {
    println(")");
    addIndent();
    n.ifStatement->accept(*this);
    removeIndent();
  }
};

void PrettyPrinter::dispatch(IfElseStatement &n) {
  print("if (");
  n.expression->accept(*this);
  
  if(dynamic_cast<Block*>(n.ifStatement.get())) {
    print(") ");
    n.ifStatement->accept(*this);
  } else {
    println(")");
    addIndent();
    n.ifStatement->accept(*this);
    removeIndent();
  }
  
  if(dynamic_cast<Block*>(n.elseStatement.get())) {
    print("else ");
    n.elseStatement->accept(*this);
  } else {
    println("else");
    addIndent();
    n.elseStatement->accept(*this);
    removeIndent();
  }
};

void PrettyPrinter::dispatch(ExpressionStatement &n) {
  n.expression->accept(*this);
  println(";");
};

void PrettyPrinter::dispatch(WhileStatement &n) {
  print("while (");
  n.expression->accept(*this);
  if(dynamic_cast<Block*>(n.statement.get())) {
    print(") ");
    n.statement->accept(*this);
  } else {
    println(") ");
    addIndent();
    n.statement->accept(*this);
    removeIndent();
  }
};

void PrettyPrinter::dispatch(LocalVariableDeclaration &n) {
  n.type->accept(*this);
  println(" " + StringTable::lookupIdentifier(n.ID) + ";");
};

void PrettyPrinter::dispatch(LocalVariableExpressionDeclaration &n) {
  n.type->accept(*this);
  println(" " + StringTable::lookupIdentifier(n.ID));
  println(" = ");
  n.expression->accept(*this);
  println(";");
};

void PrettyPrinter::dispatch(EmptyStatement &n) {
  println(";");
};

void PrettyPrinter::dispatch(ReturnStatement &n) {
  println("return;");
};

void PrettyPrinter::dispatch(ReturnExpressionStatement &n) {
  print("return ");
  n.expression->accept(*this);
  println(";");
};

void PrettyPrinter::dispatch(MethodInvocation &n) {
  print("." + StringTable::lookupIdentifier(n.ID) + "(");
  
  bool continous = false;
  for(auto const& argument:n.arguments) {
    if(continous) {
      print(", ");
    }
    argument->accept(*this);
    continous = true;
  }
  
  print(")");
};

void PrettyPrinter::dispatch(ArrayAccess &n) {
  print("[");
  n.expression->accept(*this);
  print("]");
};

void PrettyPrinter::dispatch(FieldAccess &n) {
  print("." + StringTable::lookupIdentifier(n.ID));
};

void PrettyPrinter::dispatch(AssignmentExpression &n) {
  n.expression1->accept(*this);
  print(" = ");
  n.expression2->accept(*this);
};

void PrettyPrinter::dispatch(LogicalOrExpression &n) {
  n.expression1->accept(*this);
  print(" || ");
  n.expression2->accept(*this);
};

void PrettyPrinter::dispatch(LogicalAndExpression &n) {
  n.expression1->accept(*this);
  print(" && ");
  n.expression2->accept(*this);
};

void PrettyPrinter::dispatch(EqualityExpression &n) {
  n.expression1->accept(*this);
  n.op->accept(*this);
  n.expression2->accept(*this);
};

void PrettyPrinter::dispatch(RelationalExpression &n) {
  n.expression1->accept(*this);
  n.op->accept(*this);
  n.expression2->accept(*this);
};

void PrettyPrinter::dispatch(AdditiveExpression &n) {
  n.expression1->accept(*this);
  n.op->accept(*this);
  n.expression2->accept(*this);
};

void PrettyPrinter::dispatch(MultiplicativeExpression &n) {
  n.expression1->accept(*this);
  n.op->accept(*this);
  n.expression2->accept(*this);
};

void PrettyPrinter::dispatch(CallExpression &n) {
  print(StringTable::lookupIdentifier(n.ID) + "(");
  
  bool continous = false;
  for(auto const& argument:n.arguments) {
    if(continous) {
      print(", ");
    }
    argument->accept(*this);
    continous = true;
  }
  
  print(")");
};

void PrettyPrinter::dispatch(UnaryLeftExpression &n) {
  n.op->accept(*this);
  n.expression->accept(*this);
};

void PrettyPrinter::dispatch(UnaryRightExpression &n) {
  n.expression->accept(*this);
  n.op->accept(*this);
};

void PrettyPrinter::dispatch(CNull &n) {
  print("null");
};

void PrettyPrinter::dispatch(CThis &n) {
  print("this");
};

void PrettyPrinter::dispatch(CTrue &n) {
  print("true");
};

void PrettyPrinter::dispatch(CFalse &n) {
  print("false");
};

void PrettyPrinter::dispatch(CRef &n) {
  print(StringTable::lookupIdentifier(n.ID));
};

void PrettyPrinter::dispatch(CIntegerLiteral &n) {
  print(n.integer);
};

void PrettyPrinter::dispatch(NewObject &n) {
  print("new " + StringTable::lookupIdentifier(n.ID) + "()");
};

void PrettyPrinter::dispatch(NewArray &n) {
  print("new ");
  n.type->accept(*this);
  print("[");
  n.expression->accept(*this);
  print("]");
  
  for(int i=0; i<n.arrayDepth; i++) {
    print("[]");
  }
};

void PrettyPrinter::dispatch(Equals& n) {
  print("==");
};

void PrettyPrinter::dispatch(NotEquals& n) {
  print("!=");
};

void PrettyPrinter::dispatch(LessThan& n) {
  print("<");
};

void PrettyPrinter::dispatch(LessThanOrEqual& n) {
  print("<=");
};

void PrettyPrinter::dispatch(GreaterThan& n) {
  print(">");
};

void PrettyPrinter::dispatch(GreaterThanOrEqual& n) {
  print(">=");
};

void PrettyPrinter::dispatch(Add& n) {
  print("+");
};

void PrettyPrinter::dispatch(Subtract& n) {
  print("-");
};

void PrettyPrinter::dispatch(Multiply& n) {
  print("*");
};

void PrettyPrinter::dispatch(Divide& n) {
  print("/");
};

void PrettyPrinter::dispatch(Modulo& n) {
  print("%");
};

void PrettyPrinter::dispatch(Negate& n) {
  print("!");
};

void PrettyPrinter::dispatch(Minus& n) {
  print("-");
};
