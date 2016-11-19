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

class ResolverError : public std::runtime_error
{
public:
  ResolverError(const std::string& err) : std::runtime_error(err) { }
};

inline void error(const std::string &err)
{
  throw ResolverError(err);
}



void StaticResolver::dispatch(std::shared_ptr<Program> n) {
  currentProgram = n;
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void StaticResolver::dispatch(std::shared_ptr<ClassDeclaration> n) {
  currentClassDeclaration = n;
  for (auto const& c : n->classMembers) {
    c->accept(shared_from_this());
  }
};

void StaticResolver::dispatch(std::shared_ptr<Field> n) { };

void StaticResolver::dispatch(std::shared_ptr<Method> n) {
  currentMethod = n;
  currentSymbolTable.reset(new SymbolTable());
  n->block->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<MainMethod> n) {
  currentSymbolTable.reset(new SymbolTable());
  n->block->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<Block> n) {
  currentSymbolTable->enterScope();
  
  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
  }
  
  currentSymbolTable->leaveScope();
};

void StaticResolver::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  if (currentSymbolTable->hasValueFor(n->ID)) {
    error("Multiple declarations of local variable ...");
    return;
  }
  
  currentSymbolTable->insert(n->ID, n);
};

void StaticResolver::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  if (currentSymbolTable->hasValueFor(n->ID)) {
    error("Multiple declarations of local variable ...");
    return;
  }
  
  currentSymbolTable->insert(n->ID, n);
};

void StaticResolver::dispatch(std::shared_ptr<IfStatement> n) {
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
  // method call on implicit this
  // could be handled right here
  // TODO: handle when handling MethodInvocation (consistency)
};

void StaticResolver::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<CRef> n) {
  // Try local declarations first
  if (!currentSymbolTable->lookup(n->ID, n->declaration)) {
    // try parameters
    if (currentMethod->parameterMap.count(n->ID) > 0) {
      n->declaration = currentMethod->parameterMap[n->ID];
    } else {
      // try fields (implicit this)
      if (!currentClassDeclaration->fields.count(n->ID)) {
        error("could not resolve CRef"); // todo reverse ID lookup etc.
      } else {
        n->declaration = currentClassDeclaration->fields[n->ID];
      }
    }
  }
};

void StaticResolver::dispatch(std::shared_ptr<NewObject> n) {
  n->userType->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<NewArray> n) {
  n->type->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<Type> n) {
  n->basicType->accept(shared_from_this());
};

void StaticResolver::dispatch(std::shared_ptr<UserType> n) {
  // search all class declarations
  for (auto const& c : currentProgram->classDeclarations) {
    if (c->ID == n->ID) {
      n->declaration = c;
      return;
    }
  }
  
  error("No declaration for basic type xyz"); // TODO: reverse lookup ID
};

void StaticResolver::dispatch(std::shared_ptr<Parameter> n) { };
void StaticResolver::dispatch(std::shared_ptr<TypeInt> n) { };
void StaticResolver::dispatch(std::shared_ptr<TypeBoolean> n) { };
void StaticResolver::dispatch(std::shared_ptr<TypeVoid> n) { };
void StaticResolver::dispatch(std::shared_ptr<CNull> n) { };
void StaticResolver::dispatch(std::shared_ptr<CThis> n) { };
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
