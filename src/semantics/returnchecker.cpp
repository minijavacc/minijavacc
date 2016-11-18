//
//  returnchecker.cpp
//  mjcc
//
//  Created by Markus Schlegel on 17/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "returnchecker.h"


using namespace cmpl;


std::shared_ptr<Type> voidNode() {
  return std::make_shared<Type>(std::make_shared<TypeVoid>(), 0);
}


  
void ReturnChecker::dispatch(std::shared_ptr<Program> n) {
  valid = true;
  
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
    if (!c->returns) {
      valid = false;
      break;
    }
  }
};

void ReturnChecker::dispatch(std::shared_ptr<ClassDeclaration> n) {
  n->returns = true;
  
  for (auto const& m: n->classMembers) {
    m->accept(shared_from_this()); // sets returns
    if (!m->returns) {
      n->returns = false;
      break;
    }
  }
};

void ReturnChecker::dispatch(std::shared_ptr<MainMethod> n) { n->returns = true; };
void ReturnChecker::dispatch(std::shared_ptr<Field> n) { n->returns = true; };

// Returns if either or both:
// a) has return type void
// b) the implementation block returns
void ReturnChecker::dispatch(std::shared_ptr<Method> n) {
  n->type->accept(shared_from_this());
  if (!n->type->equals(voidNode())) {
    n->block->accept(shared_from_this()); // sets returns
    n->returns = n->block->returns; // propagate upwards
  } else {
    n->returns = true;
  }
};

void ReturnChecker::dispatch(std::shared_ptr<Type> n) {
  n->type->accept(shared_from_this());
};

void ReturnChecker::dispatch(std::shared_ptr<UserType> n) { };
void ReturnChecker::dispatch(std::shared_ptr<TypeInt> n) { };
void ReturnChecker::dispatch(std::shared_ptr<TypeBoolean> n) { };
void ReturnChecker::dispatch(std::shared_ptr<TypeVoid> n) { };

void ReturnChecker::dispatch(std::shared_ptr<Parameter> n) { };

// Returns if one of the statements returns
void ReturnChecker::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
    
    // One of the statements has to return
    if (s->returns) {
      n->returns = true;
      break;
    }
  }
};

// Do not return
void ReturnChecker::dispatch(std::shared_ptr<IfStatement> n) { };
void ReturnChecker::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void ReturnChecker::dispatch(std::shared_ptr<WhileStatement> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) { };
void ReturnChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };

// Returns if both paths return
void ReturnChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
  
  n->returns = n->ifStatement->returns && n->elseStatement->returns;
};

// Always returns
void ReturnChecker::dispatch(std::shared_ptr<ReturnStatement> n) {
  n->returns = true;
};

void ReturnChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->returns = true;
};

// Unreachable
void ReturnChecker::dispatch(std::shared_ptr<MethodInvocation> n) { };
void ReturnChecker::dispatch(std::shared_ptr<ArrayAccess> n) { };
void ReturnChecker::dispatch(std::shared_ptr<FieldAccess> n) { };
void ReturnChecker::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<EqualityExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<RelationalExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CallExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CNull> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CThis> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CTrue> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CFalse> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CRef> n) { };
void ReturnChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void ReturnChecker::dispatch(std::shared_ptr<NewObject> n) { };
void ReturnChecker::dispatch(std::shared_ptr<NewArray> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Equals> n) { };
void ReturnChecker::dispatch(std::shared_ptr<NotEquals> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LessThan> n) { };
void ReturnChecker::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void ReturnChecker::dispatch(std::shared_ptr<GreaterThan> n) { };
void ReturnChecker::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Add> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Subtract> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Multiply> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Divide> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Modulo> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Negate> n) { };
void ReturnChecker::dispatch(std::shared_ptr<Minus> n) { };
