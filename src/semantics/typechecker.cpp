//
//  typechecker.cpp
//  mjcc
//
//  Created by Markus Schlegel on 18/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "typechecker.h"


using namespace cmpl;



// helpers

inline void TypeChecker::error(const std::string &err)
{
  throw TypeError(err.c_str());
}

void TypeChecker::dispatch(std::shared_ptr<Program> n) {
  for (auto const& c : n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void TypeChecker::dispatch(std::shared_ptr<ClassDeclaration> n) {
  for (auto const& c : n->classMembers) {
    c->accept(shared_from_this());
  }
};

void TypeChecker::dispatch(std::shared_ptr<MainMethod> n) {
  n->block->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<Method> n) {
  n->block->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<Field> n) { };

void TypeChecker::dispatch(std::shared_ptr<Type> n) { };

void TypeChecker::dispatch(std::shared_ptr<UserType> n) { };
void TypeChecker::dispatch(std::shared_ptr<TypeInt> n) { };
void TypeChecker::dispatch(std::shared_ptr<TypeBoolean> n) { };
void TypeChecker::dispatch(std::shared_ptr<TypeVoid> n) { };

void TypeChecker::dispatch(std::shared_ptr<Parameter> n) { };

void TypeChecker::dispatch(std::shared_ptr<Block> n) {
  for (auto const& c : n->statements) {
    c->accept(shared_from_this());
  }
};

void TypeChecker::dispatch(std::shared_ptr<IfStatement> n) {
  n->expression->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<WhileStatement> n) {
  n->expression->accept(shared_from_this());
  
  n->statement->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };

void TypeChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  n->expression->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };

void TypeChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->expression->accept(shared_from_this());
  
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<ReturnStatement> n) {
};

void TypeChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
};

void TypeChecker::dispatch(std::shared_ptr<MethodInvocation> n) { };
void TypeChecker::dispatch(std::shared_ptr<ArrayAccess> n) { };
void TypeChecker::dispatch(std::shared_ptr<FieldAccess> n) { };

void TypeChecker::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  // Check left side for lvalueness
  if (!n->expression1->isLValue) {
    error("required lvalue");
    return;
  }
  
  // TODO: check lvalue and rvalue
  
  n->type = n->expression2->type;
};

void TypeChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(booleanNode) ||
      !n->expression2->type->equals(booleanNode)) {
    error("type mismatch or");
    return;
  }
  
  n->type = n->expression2->type;
};

void TypeChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(booleanNode) ||
      !n->expression2->type->equals(booleanNode)) {
    error("type mismatch and");
    return;
  }
  
  n->type = n->expression2->type;
};

void TypeChecker::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(n->expression2->type)) {
    error("type mismatch eq");
    return;
  }
  
  n->type = n->expression2->type;
};

void TypeChecker::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(intNode) ||
      !n->expression2->type->equals(intNode)) {
    error("type mismatch rel");
    return;
  }
  
  n->type = n->expression2->type;
};

void TypeChecker::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(intNode) ||
      !n->expression2->type->equals(intNode)) {
    error("type mismatch add");
    return;
  }
  
  n->type = n->expression2->type;
};

void TypeChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(intNode) ||
      !n->expression2->type->equals(intNode)) {
    error("type mismatch mult");
    return;
  }
  
  n->type = n->expression2->type;
};

void TypeChecker::dispatch(std::shared_ptr<CallExpression> n) {
  auto decl = n->declaration.lock();
  
  if (!decl) {
    error("declaration on CallExpression has to be set before type analysis"); // TODO: better exception type
  }
  
  // Check number of arguments equals number of parameters
  if (n->arguments.size() != decl->parameters.size()) {
    error("number of arguments CallExpression <-> Method no matchy");
    return;
  }
  
  for (int i = 0; i < n->arguments.size(); i++) {
    n->arguments[i]->accept(shared_from_this());
    
    if (!n->arguments[i]->type->equals(decl->parameters[i]->type)) {
      error("type mismatch CallExpression");
      return;
    }
  }
  
  n->type = decl->type;
};

void TypeChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
  
  // case n->op is Neg => expression must be boolean
  if (dynamic_cast<Negate*>(n->op.get()) && !n->expression->type->equals(booleanNode))
  {
    error("type mismatch UnaryLeftExpression (expected boolean)");
    return;
  }
  
  // case n->op is Minus => expression must be int
  else if (dynamic_cast<Minus*>(n->op.get()) && !n->expression->type->equals(intNode))
  {
    error("type mismatch UnaryLeftExpression (expected integer)");
    return;
  }
  
  n->type = n->expression->type;
};

void TypeChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
  
  // case n->op is FieldAccess => expression must be UserType Class
  if (dynamic_cast<FieldAccess*>(n->op.get()) && !dynamic_cast<UserType*>(n->expression->type->basicType.get()))
  {
    error("type mismatch UnaryLeftExpression (expected boolean)");
    return;
  }
  
  // case n->op is ArrayAccess => expression must be int
  else if (dynamic_cast<Minus*>(n->op.get()) && !n->expression->type->equals(intNode))
  {
    error("type mismatch UnaryLeftExpression (expected integer)");
    return;
  }
  
  // case n->op is MethodInvocation => expression must be UserType Method
  else if (dynamic_cast<Minus*>(n->op.get()) && !n->expression->type->equals(intNode))
  {
    error("type mismatch UnaryLeftExpression (expected integer)");
    return;
  }
  
  
  n->type = n->expression->type;
};

void TypeChecker::dispatch(std::shared_ptr<CNull> n) {
  // TODO
};

void TypeChecker::dispatch(std::shared_ptr<CThis> n) { };
void TypeChecker::dispatch(std::shared_ptr<CTrue> n) { };
void TypeChecker::dispatch(std::shared_ptr<CFalse> n) { };

void TypeChecker::dispatch(std::shared_ptr<CRef> n) {
  // follow pointer to decl
  // return type of decl
};

void TypeChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void TypeChecker::dispatch(std::shared_ptr<NewObject> n) { };
void TypeChecker::dispatch(std::shared_ptr<NewArray> n) { };
void TypeChecker::dispatch(std::shared_ptr<Equals> n) { };
void TypeChecker::dispatch(std::shared_ptr<NotEquals> n) { };
void TypeChecker::dispatch(std::shared_ptr<LessThan> n) { };
void TypeChecker::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void TypeChecker::dispatch(std::shared_ptr<GreaterThan> n) { };
void TypeChecker::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void TypeChecker::dispatch(std::shared_ptr<Add> n) { };
void TypeChecker::dispatch(std::shared_ptr<Subtract> n) { };
void TypeChecker::dispatch(std::shared_ptr<Multiply> n) { };
void TypeChecker::dispatch(std::shared_ptr<Divide> n) { };
void TypeChecker::dispatch(std::shared_ptr<Modulo> n) { };
void TypeChecker::dispatch(std::shared_ptr<Negate> n) { };
void TypeChecker::dispatch(std::shared_ptr<Minus> n) { };
