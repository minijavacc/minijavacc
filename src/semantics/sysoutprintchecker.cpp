#include "sysoutprintchecker.h"

using namespace cmpl;

inline void SysOutPrintChecker::error(const std::string &err)
{
  throw SysOutPrintError(err.c_str());
}

std::shared_ptr<Expression> SysOutPrintChecker::convertToStaticLibraryCallExpressionNode()
{
  std::shared_ptr<Expression> node = std::make_shared<StaticLibraryCallExpression>(printlnParamExpr);
  
  return node;
}

void SysOutPrintChecker::dispatch(std::shared_ptr<Program> n) {
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<ClassDeclaration> n) {
  currentClassDeclaration = n;
  
  for (auto const& m: n->classMembers) {
    m->accept(shared_from_this());
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<MainMethod> n) {
  mainMethod = n;
  n->block->accept(shared_from_this());
  mainMethod = nullptr;
};

void SysOutPrintChecker::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<CRef> n) {
  if (StringTable::lookupIdentifier(n->ID) == "System")
  {
    assert(currentClassDeclaration != nullptr);
    assert(mainMethod || (currentMethod != nullptr));
    
    bool identifierSystemExists = false;
    LocalVariableDeclaration* lvc;
    LocalVariableExpressionDeclaration* lvec;
    
    // check if "System" exists as field, parameter of local variable
    // scopes not checked here, but the language description says:
    //   "Ist kein Bezeichner System bekannt, so erzeugt dieser Ausdruck einen 
    //    Aufruf der System.out.println-Funktion in der Standardbibliothek."
    if (currentClassDeclaration->fields.count(n->ID) == 0)
    {
      if (mainMethod)
      {
        for (const std::shared_ptr<Node> &l : mainMethod->localVariables)
        {
          if ((lvc = dynamic_cast<LocalVariableDeclaration*>(l.get())) && 
            (StringTable::lookupIdentifier(lvc->ID) == "System"))
          {
            identifierSystemExists = true;
            break;
          }
          else if ((lvec = dynamic_cast<LocalVariableExpressionDeclaration*>(l.get())) && 
            (StringTable::lookupIdentifier(lvec->ID) == "System"))
          {
            identifierSystemExists = true;
            break;
          }
        }
      }
      else if (currentMethod->parameterMap.count(n->ID) == 0)
      {
        for (const std::shared_ptr<Node> &l : currentMethod->localVariables)
        {
          if ((lvc = dynamic_cast<LocalVariableDeclaration*>(l.get())) && 
            (StringTable::lookupIdentifier(lvc->ID) == "System"))
          {
            identifierSystemExists = true;
            break;
          }
          else if ((lvec = dynamic_cast<LocalVariableExpressionDeclaration*>(l.get())) && 
            (StringTable::lookupIdentifier(lvec->ID) == "System"))
          {
            identifierSystemExists = true;
            break;
          }
        }
      }
      
      if (!identifierSystemExists)
      {
        cRefSystem = n;
      }
    }
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<CThis> n) {
};


void SysOutPrintChecker::dispatch(std::shared_ptr<Method> n) {
  currentMethod = n;
  n->block->accept(shared_from_this());
};


void SysOutPrintChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->expression->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<IfStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<WhileStatement> n) {
  n->expression->accept(shared_from_this());
  n->statement->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->expression->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<MethodInvocation> n) {
  if (StringTable::lookupIdentifier(n->ID) == "println")
  {
    if (n->arguments.size() != 1)
    {
      return;
    }
    
    printlnParamExpr = n->arguments[0];
    methodInvocationPrintln = n;
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<ArrayAccess> n) {
  n->expression->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (n->expression1 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression1 = convertToStaticLibraryCallExpressionNode();
  }
  else if (n->expression2 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression2 = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (n->expression1 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression1 = convertToStaticLibraryCallExpressionNode();
  }
  else if (n->expression2 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression2 = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (n->expression1 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression1 = convertToStaticLibraryCallExpressionNode();
  }
  else if (n->expression2 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression2 = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (n->expression1 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression1 = convertToStaticLibraryCallExpressionNode();
  }
  else if (n->expression2 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression2 = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (n->expression1 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression1 = convertToStaticLibraryCallExpressionNode();
  }
  else if (n->expression2 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression2 = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (n->expression1 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression1 = convertToStaticLibraryCallExpressionNode();
  }
  else if (n->expression2 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression2 = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (n->expression1 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression1 = convertToStaticLibraryCallExpressionNode();
  }
  else if (n->expression2 == unaryRightExpressionSystemOutPrintln)
  {
    n->expression2 = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<CallExpression> n) {
  for(int i = 0; i < n->arguments.size(); i++) {
    n->arguments[i]->accept(shared_from_this());
    
    if (n->arguments[i] == unaryRightExpressionSystemOutPrintln)
    {
      n->arguments[i] = convertToStaticLibraryCallExpressionNode();
    }
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
  n->op->accept(shared_from_this());
  
  
  if (n->op == fieldAccessOut && n->expression == cRefSystem)
  {
    exprSystemOut = n;
  }
  else if (n->op == methodInvocationPrintln && n->expression == exprSystemOut)
  {
    // found subtree
    unaryRightExpressionSystemOutPrintln = n;
  }
  else if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};


void SysOutPrintChecker::dispatch(std::shared_ptr<NewArray> n) {
  n->expression->accept(shared_from_this());
  
  if (n->expression == unaryRightExpressionSystemOutPrintln)
  {
    n->expression = convertToStaticLibraryCallExpressionNode();
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<StaticLibraryCallExpression> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<ReturnStatement> n) { };

void SysOutPrintChecker::dispatch(std::shared_ptr<FieldAccess> n) {
  if (StringTable::lookupIdentifier(n->ID) == "out")
  {
    fieldAccessOut = n;
  }
};

void SysOutPrintChecker::dispatch(std::shared_ptr<NewObject> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Type> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<FakeType> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<NullType> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<UserType> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Field> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Parameter> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<TypeInt> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<TypeBoolean> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<TypeVoid> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<CNull> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<CTrue> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<CFalse> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Equals> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<NotEquals> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<LessThan> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<GreaterThan> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Add> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Subtract> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Multiply> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Divide> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Modulo> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Negate> n) { };
void SysOutPrintChecker::dispatch(std::shared_ptr<Minus> n) { };
