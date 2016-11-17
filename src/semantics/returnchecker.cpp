//
//  returnchecker.cpp
//  mjcc
//
//  Created by Markus Schlegel on 17/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "returnchecker.h"


using namespace cmpl;


  
void ReturnChecker::dispatch(Program &n) {
  valid = true;
  
  for(auto const& c: n.classDeclarations) {
    c->accept(*this);
    if (!c->returns) {
      valid = false;
      break;
    }
  }
};

void ReturnChecker::dispatch(ClassDeclaration &n) {
  n.returns = true;
  
  for (auto const& m: n.classMembers) {
    m->accept(*this); // sets returns
    if (!m->returns) {
      n.returns = false;
      break;
    }
  }
};

void ReturnChecker::dispatch(MainMethod &n) { n.returns = true; };
void ReturnChecker::dispatch(Field &n) { n.returns = true; };

// Returns if either or both:
// a) has return type void
// b) the implementation block returns
void ReturnChecker::dispatch(Method &n) {
  n.type->accept(*this); // sets isVoid
  if (!n.type->isVoid) {
    n.block->accept(*this); // sets returns
    n.returns = n.block->returns; // propagate upwards
  } else {
    n.returns = true;
  }
};

void ReturnChecker::dispatch(Type &n) {
  n.type->accept(*this);
  n.isVoid = n.type->isVoid;
  
};

void ReturnChecker::dispatch(UserType &n) { };
void ReturnChecker::dispatch(TypeInt &n) { };
void ReturnChecker::dispatch(TypeBoolean &n) { };

void ReturnChecker::dispatch(TypeVoid &n) {
  n.isVoid = true;
};

void ReturnChecker::dispatch(Parameter &n) { };

// Returns if one of the statements returns
void ReturnChecker::dispatch(Block &n) {
  for (auto const& s: n.statements) {
    s->accept(*this);
    
    // One of the statements has to return
    if (s->returns) {
      n.returns = true;
      break;
    }
  }
};

// Do not return
void ReturnChecker::dispatch(IfStatement &n) { };
void ReturnChecker::dispatch(ExpressionStatement &n) { };
void ReturnChecker::dispatch(WhileStatement &n) { };
void ReturnChecker::dispatch(LocalVariableDeclaration &n) { };
void ReturnChecker::dispatch(LocalVariableExpressionDeclaration &n) { };
void ReturnChecker::dispatch(EmptyStatement &n) { };

// Returns if both paths return
void ReturnChecker::dispatch(IfElseStatement &n) {
  n.ifStatement->accept(*this);
  n.elseStatement->accept(*this);
  
  n.returns = n.ifStatement->returns && n.elseStatement->returns;
};

// Always returns
void ReturnChecker::dispatch(ReturnStatement &n) {
  n.returns = true;
};

void ReturnChecker::dispatch(ReturnExpressionStatement &n) {
  n.returns = true;
};

// Unreachable
void ReturnChecker::dispatch(MethodInvocation &n) { };
void ReturnChecker::dispatch(ArrayAccess &n) { };
void ReturnChecker::dispatch(FieldAccess &n) { };
void ReturnChecker::dispatch(AssignmentExpression &n) { };
void ReturnChecker::dispatch(LogicalOrExpression &n) { };
void ReturnChecker::dispatch(LogicalAndExpression &n) { };
void ReturnChecker::dispatch(EqualityExpression &n) { };
void ReturnChecker::dispatch(RelationalExpression &n) { };
void ReturnChecker::dispatch(AdditiveExpression &n) { };
void ReturnChecker::dispatch(MultiplicativeExpression &n) { };
void ReturnChecker::dispatch(CallExpression &n) { };
void ReturnChecker::dispatch(UnaryLeftExpression &n) { };
void ReturnChecker::dispatch(UnaryRightExpression &n) { };
void ReturnChecker::dispatch(CNull &n) { };
void ReturnChecker::dispatch(CThis &n) { };
void ReturnChecker::dispatch(CTrue &n) { };
void ReturnChecker::dispatch(CFalse &n) { };
void ReturnChecker::dispatch(CRef &n) { };
void ReturnChecker::dispatch(CIntegerLiteral &n) { };
void ReturnChecker::dispatch(NewObject &n) { };
void ReturnChecker::dispatch(NewArray &n) { };
void ReturnChecker::dispatch(Equals& n) { };
void ReturnChecker::dispatch(NotEquals& n) { };
void ReturnChecker::dispatch(LessThan& n) { };
void ReturnChecker::dispatch(LessThanOrEqual& n) { };
void ReturnChecker::dispatch(GreaterThan& n) { };
void ReturnChecker::dispatch(GreaterThanOrEqual& n) { };
void ReturnChecker::dispatch(Add& n) { };
void ReturnChecker::dispatch(Subtract& n) { };
void ReturnChecker::dispatch(Multiply& n) { };
void ReturnChecker::dispatch(Divide& n) { };
void ReturnChecker::dispatch(Modulo& n) { };
void ReturnChecker::dispatch(Negate& n) { };
void ReturnChecker::dispatch(Minus& n) { };
