//
//  staticresolver.cpp
//  mjcc
//
//  Created by Markus Schlegel on 18/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "staticresolver.h"
#include <iostream>

using namespace cmpl;


// helpers

inline void StaticResolver::error(const std::string &err)
{
  throw ResolverError(("staticresolver: " + err).c_str());
}

inline void StaticResolver::error(const std::string &err, const std::shared_ptr<Node> &n)
{
  throw ResolverError(("typechecker: " + err + ": " + Checker::printNode(n)).c_str());
}

void StaticResolver::dispatch(std::shared_ptr<Program> n) {
  currentProgram = n;
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void StaticResolver::dispatch(std::shared_ptr<ClassDeclaration> n) {
  // set declaration pointer in own type
  auto userType = dynamic_cast<UserType*>(n->type->basicType.get());
  
  userType->declaration = n;
  
  currentClassDeclaration = n;
  for (auto const& c : n->classMembers) {
    c->accept(shared_from_this());
  }
};

void StaticResolver::dispatch(std::shared_ptr<Field> n) {
  n->type->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<Method> n) {
  n->type->accept(shared_from_this());
  
  currentMethod = n;
  currentSymbolTable.reset(new SymbolTable());
  currentSymbolTable->enterScope();
  
  for (auto const& p : n->parameters)
  {
    p->accept(shared_from_this());
  }
  
  n->block->accept(shared_from_this());
  
  currentSymbolTable->leaveScope();
};

void StaticResolver::dispatch(std::shared_ptr<MainMethod> n) {
  currentSymbolTable.reset(new SymbolTable());
  currentSymbolTable->enterScope();
  currentSymbolTable->insert(n->parameterID, n); // mainmethodchecker assures that parameterID is never used
  n->block->accept(shared_from_this());
  currentSymbolTable->leaveScope();
};

void StaticResolver::dispatch(std::shared_ptr<Block> n) {
  currentSymbolTable->enterScope();
  
  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
  }
  
  currentSymbolTable->leaveScope();
};

void StaticResolver::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  n->type->accept(shared_from_this());
  
  if (currentSymbolTable->hasValueFor(n->ID)) {
    error("Multiple declarations of local variable ...", n);
  }
  
  currentSymbolTable->insert(n->ID, n);
};

void StaticResolver::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->type->accept(shared_from_this());
  // declaration can already be used in following expression
  if (currentSymbolTable->hasValueFor(n->ID)) {
    error("Multiple declarations of local variable ...", n);
    return;
  }
  currentSymbolTable->insert(n->ID, n);
  
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<IfStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<WhileStatement> n) {
  n->expression->accept(shared_from_this());
  n->statement->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<EmptyStatement> n) { };
void StaticResolver::dispatch(std::shared_ptr<ReturnStatement> n) { };

void StaticResolver::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<MethodInvocation> n) {
  for (auto const& a : n->arguments) {
    a->accept(shared_from_this());
  }
};

void StaticResolver::dispatch(std::shared_ptr<ArrayAccess> n) {
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<FieldAccess> n) {
  // requires (partial) type analysis first
};

void StaticResolver::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<LogicalOrExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<LogicalAndExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<CallExpression> n) {
  // method call on implicit this -> look for method in this class
  
  if (currentClassDeclaration->methods.count(n->ID) != 1)
  {
    error("CallExpression to undefined method", n);
  }
  
  n->declaration = currentClassDeclaration->methods[n->ID];
  
  for (auto const& a : n->arguments) {
    a->accept(shared_from_this());
  }
};

void StaticResolver::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
  n->op->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<CRef> n) {
  // Try local declarations first (including parameters)
  if (!currentSymbolTable->lookup(n->ID, n->declaration)) {
    // try fields (implicit this)
    if (currentClassDeclaration->fields.count(n->ID)!=1) {
      error("could not resolve CRef '" + StringTable::lookupIdentifier(n->ID) + "'");
    } else {
      n->declaration = currentClassDeclaration->fields[n->ID];
    }
  } // else: declaration has successfully been set implicitly.
  
  assert(n->declaration.lock().get() != nullptr);
};

void StaticResolver::dispatch(std::shared_ptr<NewObject> n) {
  n->userType->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<NewArray> n) {
  n->expression->accept(shared_from_this());
  n->type->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<SLCPrintlnExpression> n) {
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<SLCWriteExpression> n) {
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<SLCFlushExpression> n) { };

void StaticResolver::dispatch(std::shared_ptr<SLCReadExpression> n) { };

void StaticResolver::dispatch(std::shared_ptr<Type> n) {
  n->basicType->accept(shared_from_this());
};


void StaticResolver::dispatch(std::shared_ptr<UserType> n) {
  // search all class declarations
  for (auto const& c : currentProgram->classDeclarations) {
    if (c->ID == n->ID) {
      n->declaration = c;
      assert(c != nullptr);
      return;
    }
  }
  
  error("No declaration for basic type " + StringTable::lookupIdentifier(n->ID), n);
};

void StaticResolver::dispatch(std::shared_ptr<CThis> n) {
  n->declaration = currentClassDeclaration;
};


void StaticResolver::dispatch(std::shared_ptr<Parameter> n) {
  if (currentSymbolTable->hasValueFor(n->ID)) {
    error("Multiple declarations of parameter in method signature ...", n);
    return;
  }
  
  n->type->accept(shared_from_this());
  
  currentSymbolTable->insert(n->ID, n);
};

void StaticResolver::dispatch(std::shared_ptr<FakeType> n) { };
void StaticResolver::dispatch(std::shared_ptr<NullType> n) { };
void StaticResolver::dispatch(std::shared_ptr<TypeInt> n) { };
void StaticResolver::dispatch(std::shared_ptr<TypeBoolean> n) { };
void StaticResolver::dispatch(std::shared_ptr<TypeVoid> n) { };
void StaticResolver::dispatch(std::shared_ptr<CNull> n) { };
void StaticResolver::dispatch(std::shared_ptr<CTrue> n) { };
void StaticResolver::dispatch(std::shared_ptr<CFalse> n) { };
void StaticResolver::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void StaticResolver::dispatch(std::shared_ptr<Equals> n) { };
void StaticResolver::dispatch(std::shared_ptr<NotEquals> n) { };
void StaticResolver::dispatch(std::shared_ptr<LessThan> n) { };
void StaticResolver::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void StaticResolver::dispatch(std::shared_ptr<GreaterThan> n) { };
void StaticResolver::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void StaticResolver::dispatch(std::shared_ptr<Add> n) { };
void StaticResolver::dispatch(std::shared_ptr<Subtract> n) { };
void StaticResolver::dispatch(std::shared_ptr<Multiply> n) { };
void StaticResolver::dispatch(std::shared_ptr<Divide> n) { };
void StaticResolver::dispatch(std::shared_ptr<Modulo> n) { };
void StaticResolver::dispatch(std::shared_ptr<Negate> n) { };
void StaticResolver::dispatch(std::shared_ptr<Minus> n) { };
