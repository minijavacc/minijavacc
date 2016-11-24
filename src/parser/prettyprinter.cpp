#include "prettyprinter.h"

#include <assert.h>
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
  if(indentsPrinted) {
    printStream << s << "\n";
  } else {
    printStream << indents << s << "\n";
  }
  
  indentsPrinted = false;
}

void PrettyPrinter::addIndent()
{
  indents.append(indent);
}

void PrettyPrinter::removeIndent()
{
  indents.erase(indents.size() - indent.size(), indent.size());
}

std::ostream& PrettyPrinter::getStream()
{
  return printStream;
}





void PrettyPrinter::dispatch(std::shared_ptr<Program> n) {
  // sort classes by alphabetical order
  std::sort(n->classDeclarations.begin(), n->classDeclarations.end(), 
    [](const std::shared_ptr<ClassDeclaration> &a, const std::shared_ptr<ClassDeclaration> &b) -> bool{
      return StringTable::lookupIdentifier(b->ID).compare(StringTable::lookupIdentifier(a->ID)) > 0;
  });
  
  for(auto const& classDeclaration : n->classDeclarations) {
    classDeclaration->accept(shared_from_this());
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<ClassDeclaration> n) {
  println("class "+ StringTable::lookupIdentifier(n->ID) + " {");

  // sort the classMembers
  // first methods then fields, each of them in alphabetical order
  
  std::sort(n->classMembers.begin(), n->classMembers.end(), 
    [](const std::shared_ptr<ClassMember> &a, const std::shared_ptr<ClassMember> &b) -> bool{
      Field* a_p;
      Field* b_p;
      
      if ((a_p = dynamic_cast<Field*>(a.get())) && (b_p = dynamic_cast<Field*>(b.get())))
      {
        // a and b are both Fields
        
        // get identifier strings and compare them
        return StringTable::lookupIdentifier(a_p->ID).compare(StringTable::lookupIdentifier(b_p->ID)) > 0;
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
        return a_s.compare(b_s) > 0;
      }
      else if (dynamic_cast<Field*>(a.get()) && 
        (dynamic_cast<Method*>(b.get()) || dynamic_cast<MainMethod*>(b.get())))
      {
        // a is field and b is Method or MainMethod
        return true;
      }
      else
      {
        // a is Method or MainMethod and b is Field
        return false;
      }
  });

  addIndent();
  
  for(auto const& classMember : n->classMembers) {
    classMember->accept(shared_from_this());
  }
  
  removeIndent();
  println("}");
};

void PrettyPrinter::dispatch(std::shared_ptr<MainMethod> n) {
  print("public static void " + StringTable::lookupIdentifier(n->ID) + "(String[] " + StringTable::lookupIdentifier(n->parameterID) + ") ");
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
  
  print(") ");
  n->block->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<Type> n) {
  n->basicType->accept(shared_from_this());
  for(int i=0; i<n->arrayDepth; i++) {
    print("[]");
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<FakeType> n) {
  assert(false);
};

void PrettyPrinter::dispatch(std::shared_ptr<NullType> n) {
  assert(false);
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
  print(" " + StringTable::lookupIdentifier(n->ID));
};

void PrettyPrinter::dispatch(std::shared_ptr<Block> n) {
  if (n->statements.size() > 0)
  {
    println("{");
    addIndent();
    for(auto const& statement : n->statements) {
      statement->accept(shared_from_this());
    }
    removeIndent();
    println("}");
  }
  else
  {
    println("{ }");
  }
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
  print(" " + StringTable::lookupIdentifier(n->ID));
  print(" = ");
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
  print(" ");
  n->op->accept(shared_from_this());
  print(" ");
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  print(" ");
  n->op->accept(shared_from_this());
  print(" ");
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  print(" ");
  n->op->accept(shared_from_this());
  print(" ");
  n->expression2->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  print(" ");
  n->op->accept(shared_from_this());
  print(" ");
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
  print("(");
  n->expression->accept(shared_from_this());
  print(")");
};

void PrettyPrinter::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  print("(");
  n->expression->accept(shared_from_this());
  print(")");
  n->op->accept(shared_from_this());
};

void PrettyPrinter::dispatch(std::shared_ptr<CNull> n) { print("null"); };
void PrettyPrinter::dispatch(std::shared_ptr<CThis> n) { print("this"); };
void PrettyPrinter::dispatch(std::shared_ptr<CTrue> n) { print("true"); };
void PrettyPrinter::dispatch(std::shared_ptr<CFalse> n) { print("false"); };

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
  if (!n->type)
    throw std::runtime_error("fatal error: NewArray node has empty type");
  if (!n->type->basicType)
    throw std::runtime_error("fatal error: NewArray node has empty type");
  
  print("new ");
  n->type->basicType->accept(shared_from_this());
  print("[");
  n->expression->accept(shared_from_this());
  print("]");
  
  for(int i = 0; i < 0 /*n->type->arrayDepth*/; i++) {
    print("[]");
  }
};

void PrettyPrinter::dispatch(std::shared_ptr<Equals> n)             { print("=="); };
void PrettyPrinter::dispatch(std::shared_ptr<NotEquals> n)          { print("!="); };
void PrettyPrinter::dispatch(std::shared_ptr<LessThan> n)           { print("<"); };
void PrettyPrinter::dispatch(std::shared_ptr<LessThanOrEqual> n)    { print("<="); };
void PrettyPrinter::dispatch(std::shared_ptr<GreaterThan> n)        { print(">"); };
void PrettyPrinter::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { print(">="); };
void PrettyPrinter::dispatch(std::shared_ptr<Add> n)                { print("+"); };
void PrettyPrinter::dispatch(std::shared_ptr<Subtract> n)           { print("-"); };
void PrettyPrinter::dispatch(std::shared_ptr<Multiply> n)           { print("*"); };
void PrettyPrinter::dispatch(std::shared_ptr<Divide> n)             { print("/"); };
void PrettyPrinter::dispatch(std::shared_ptr<Modulo> n)             { print("%"); };
void PrettyPrinter::dispatch(std::shared_ptr<Negate> n)             { print("!"); };
void PrettyPrinter::dispatch(std::shared_ptr<Minus> n)              { print("-"); };

