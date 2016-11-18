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





void PrettyPrinter::dispatch(std::shared_ptr<Program> n) {
  for(auto const& classDeclaration : n->classDeclarations) {
    classDeclaration->accept(shared_from_this());
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<ClassDeclaration> n) {
  println("class "+ StringTable::lookupIdentifier(n->ID) + " {");

  addIndent();
  
  for(auto const& classMember : n->classMembers) {
    classMember->accept(shared_from_this());
  }
  
  removeIndent();
  println("}");
};

void PrettyPrinter::dispatch(std::shared_ptr<MainMethod> n) {
  print("public static void " + StringTable::lookupIdentifier(n->ID) + "(String[] " + StringTable::lookupIdentifier(n->parameterID) + ")");
  n->block->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<Field> n) {
  print("public ");
  n->type->accept(shared_from_this());
  println(" " + StringTable::lookupIdentifier(n->ID) + ";");
};

void PrettyPrinter::dispatch(std::shared_ptr<Method> n) {
  print("public ");
  n->type->accept(shared_from_this());
  print(" ");
  print(StringTable::lookupIdentifier(n->ID) + "(");
  
  bool continous = false;
  for(auto const& parameter:n->parameters) {
    if(continous) {
      print(", ");
    }
    parameter->accept(shared_from_this());
    continous = true;
  }
  
  print(")");
  n->block->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<Type> n) {
  n->type->accept(shared_from_this());
  for(int i=0; i<n->arrayDepth; i++) {
    print("[]");
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<UserType> n) {
  print(StringTable::lookupIdentifier(n->ID));
};

void PrettyPrinter::dispatch(std::shared_ptr<TypeInt> n) {
  print("int");
};

void PrettyPrinter::dispatch(std::shared_ptr<TypeBoolean> n) {
  print("boolean");
};

void PrettyPrinter::dispatch(std::shared_ptr<TypeVoid> n) {
  print("void");
};

void PrettyPrinter::dispatch(std::shared_ptr<Parameter> n) {
  n->type->accept(shared_from_this());
  println(" " + StringTable::lookupIdentifier(n->ID));
};

void PrettyPrinter::dispatch(std::shared_ptr<Block> n) {
  println("{");
  addIndent();
  for(auto const& statement : n->statements) {
    statement->accept(shared_from_this());
  }
  removeIndent();
  println("}");
};

void PrettyPrinter::dispatch(std::shared_ptr<IfStatement> n) {
  print("if (");
  n->expression->accept(shared_from_this());
  
  if(dynamic_cast<Block*>(n->ifStatement.get())) {
    print(") ");
    n->ifStatement->accept(shared_from_this());
  } else {
    println(")");
    addIndent();
    n->ifStatement->accept(shared_from_this());
    removeIndent();
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<IfElseStatement> n) {
  print("if (");
  n->expression->accept(shared_from_this());
  
  if(dynamic_cast<Block*>(n->ifStatement.get())) {
    print(") ");
    n->ifStatement->accept(shared_from_this());
  } else {
    println(")");
    addIndent();
    n->ifStatement->accept(shared_from_this());
    removeIndent();
  }
  
  if(dynamic_cast<Block*>(n->elseStatement.get())) {
    print("else ");
    n->elseStatement->accept(shared_from_this());
  } else {
    println("else");
    addIndent();
    n->elseStatement->accept(shared_from_this());
    removeIndent();
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
  println(";");
};

void PrettyPrinter::dispatch(std::shared_ptr<WhileStatement> n) {
  print("while (");
  n->expression->accept(shared_from_this());
  if(dynamic_cast<Block*>(n->statement.get())) {
    print(") ");
    n->statement->accept(shared_from_this());
  } else {
    println(") ");
    addIndent();
    n->statement->accept(shared_from_this());
    removeIndent();
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  n->type->accept(shared_from_this());
  println(" " + StringTable::lookupIdentifier(n->ID) + ";");
};

void PrettyPrinter::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->type->accept(shared_from_this());
  println(" " + StringTable::lookupIdentifier(n->ID));
  println(" = ");
  n->expression->accept(shared_from_this());
  println(";");
};

void PrettyPrinter::dispatch(std::shared_ptr<EmptyStatement> n) {
  println(";");
};

void PrettyPrinter::dispatch(std::shared_ptr<ReturnStatement> n) {
  println("return;");
};

void PrettyPrinter::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  print("return ");
  n->expression->accept(shared_from_this());
  println(";");
};

void PrettyPrinter::dispatch(std::shared_ptr<MethodInvocation> n) {
  print("." + StringTable::lookupIdentifier(n->ID) + "(");
  
  bool continous = false;
  for(auto const& argument:n->arguments) {
    if(continous) {
      print(", ");
    }
    argument->accept(shared_from_this());
    continous = true;
  }
  
  print(")");
};

void PrettyPrinter::dispatch(std::shared_ptr<ArrayAccess> n) {
  print("[");
  n->expression->accept(shared_from_this());
  print("]");
};

void PrettyPrinter::dispatch(std::shared_ptr<FieldAccess> n) {
  print("." + StringTable::lookupIdentifier(n->ID));
};

void PrettyPrinter::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression1->accept(shared_from_this());
  print(" = ");
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<LogicalOrExpression> n) {
  n->expression1->accept(shared_from_this());
  print(" || ");
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<LogicalAndExpression> n) {
  n->expression1->accept(shared_from_this());
  print(" && ");
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->op->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->op->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->op->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->op->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<CallExpression> n) {
  print(StringTable::lookupIdentifier(n->ID) + "(");
  
  bool continous = false;
  for(auto const& argument:n->arguments) {
    if(continous) {
      print(", ");
    }
    argument->accept(shared_from_this());
    continous = true;
  }
  
  print(")");
};

void PrettyPrinter::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->op->accept(shared_from_this());
  n->expression->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
  n->op->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<CNull> n) {
  print("null");
};

void PrettyPrinter::dispatch(std::shared_ptr<CThis> n) {
  print("this");
};

void PrettyPrinter::dispatch(std::shared_ptr<CTrue> n) {
  print("true");
};

void PrettyPrinter::dispatch(std::shared_ptr<CFalse> n) {
  print("false");
};

void PrettyPrinter::dispatch(std::shared_ptr<CRef> n) {
  print(StringTable::lookupIdentifier(n->ID));
};

void PrettyPrinter::dispatch(std::shared_ptr<CIntegerLiteral> n) {
  print(n->integer);
};

void PrettyPrinter::dispatch(std::shared_ptr<NewObject> n) {
  print("new " + StringTable::lookupIdentifier(n->userType->ID) + "()");
};

void PrettyPrinter::dispatch(std::shared_ptr<NewArray> n) {
  print("new ");
  n->type->accept(shared_from_this());
  print("[");
  n->expression->accept(shared_from_this());
  print("]");
  
  for(int i=0; i<n->arrayDepth; i++) {
    print("[]");
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<Equals> n) {
  print("==");
};

void PrettyPrinter::dispatch(std::shared_ptr<NotEquals> n) {
  print("!=");
};

void PrettyPrinter::dispatch(std::shared_ptr<LessThan> n) {
  print("<");
};

void PrettyPrinter::dispatch(std::shared_ptr<LessThanOrEqual> n) {
  print("<=");
};

void PrettyPrinter::dispatch(std::shared_ptr<GreaterThan> n) {
  print(">");
};

void PrettyPrinter::dispatch(std::shared_ptr<GreaterThanOrEqual> n) {
  print(">=");
};

void PrettyPrinter::dispatch(std::shared_ptr<Add> n) {
  print("+");
};

void PrettyPrinter::dispatch(std::shared_ptr<Subtract> n) {
  print("-");
};

void PrettyPrinter::dispatch(std::shared_ptr<Multiply> n) {
  print("*");
};

void PrettyPrinter::dispatch(std::shared_ptr<Divide> n) {
  print("/");
};

void PrettyPrinter::dispatch(std::shared_ptr<Modulo> n) {
  print("%");
};

void PrettyPrinter::dispatch(std::shared_ptr<Negate> n) {
  print("!");
};

void PrettyPrinter::dispatch(std::shared_ptr<Minus> n) {
  print("-");
};
