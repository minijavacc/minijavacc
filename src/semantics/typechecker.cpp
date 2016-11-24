//
//  typechecker.cpp
//  mjcc
//
//  Created by Markus Schlegel on 18/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "typechecker.h"
#include "singleton.h"

using namespace cmpl;

// helpers

inline void TypeChecker::error(const std::string &err)
{
  throw TypeError(("typechecker: " + err).c_str());
}

inline void TypeChecker::error(const std::string &err, const std::shared_ptr<Node> &n)
{
  throw TypeError(("typechecker: " + err + ": " + Checker::printNode(n)).c_str());
}

inline void TypeChecker::fatalError(const std::string &err, const std::shared_ptr<Node> &n)
{
  throw std::logic_error("typechecker: #fatal " + err + ": " + Checker::printNode(n));
}

void TypeChecker::dispatch(std::shared_ptr<Program> n) {
  currentProgram = n;
  
  for (auto const& c : n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void TypeChecker::dispatch(std::shared_ptr<ClassDeclaration> n) {
  currentClassDeclaration = n;
  // type already set by parser
  
  for (auto const& c : n->classMembers) {
    c->accept(shared_from_this());
  }
};

void TypeChecker::dispatch(std::shared_ptr<MainMethod> n) {
  // currentMethod doesn't need to be set
  n->block->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<Method> n) {
  currentMethod = n;
  
  n->block->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<Field> n) { };

void TypeChecker::dispatch(std::shared_ptr<Type> n) { };

void TypeChecker::dispatch(std::shared_ptr<FakeType> n) { };
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
  
  // check if expression is boolean
  if (!n->expression->type->equals(booleanNode))
  {
    error("expression in IfElseStatement must be of type boolean");
  }
};

void TypeChecker::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<WhileStatement> n) {
  n->expression->accept(shared_from_this());
  
  n->statement->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  // type has been set by parser
  
  // REVIEW: declares every LocalVariableDeclaration an lvalue type?
  // set isLValue=true
  n->isLValue = true;
};

void TypeChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  // type has been set by parser
  
  // get type of expression
  n->expression->accept(shared_from_this());
  
  // check if type matches the type of the expression
  if (!n->type->equals(n->expression->type))
  {
    error("expression in LocalVariableExpressionDeclaration has wrong type", n);
  }
  
  // REVIEW: declares every LocalVariableExpressionDeclaration an lvalue type?
  // set isLValue=true
  n->isLValue = true;
};

void TypeChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };

void TypeChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
  
  // check if expression is boolean
  if (!n->expression->type->equals(booleanNode))
  {
    error("expression in IfElseStatement must be of type boolean", n->expression);
  }
};

void TypeChecker::dispatch(std::shared_ptr<ReturnStatement> n) { };

void TypeChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->expression->accept(shared_from_this());
    //check first if its a void method, then return EXPR is invalid in java!
  if(currentMethod->type->equals(voidNode))
  {
    error("void methods can't return an expression", n);
  }
  else if (!currentMethod->type->equals(n->expression->type))
  {
    error("expression type in return statement doesn't match method return type", n);
  }
};

void TypeChecker::dispatch(std::shared_ptr<MethodInvocation> n) {
  auto userType = dynamic_cast<UserType*>(tmpExpression->type->basicType.get());
  
  if (!userType)
  {
    error("method invocation on non-UserType", n);
  }
  
  auto classDecl = userType->declaration.lock();
  
  if (!classDecl)
  {
    fatalError("declaration missing for UserType in MethodInvocation", n);
  }
  
  // check for method ID in class given by expression
  if (classDecl->methods.count(n->ID) != 1)
  {
    error("method invocation to unknown method (could not find method in UserType)");
  }
  
  auto methodDecl = classDecl->methods[n->ID].lock();
  
  // check arguments
  // Check number of arguments equals number of parameters
  if (n->arguments.size() != methodDecl->parameters.size()) {
    error("number of arguments MethodInvocation <-> Method does not match", n);
  }
  
  for (int i = 0; i < n->arguments.size(); i++) {
    n->arguments[i]->accept(shared_from_this());
    
    if (!n->arguments[i]->type->equals(methodDecl->parameters[i]->type)) {
      error("type mismatch of MethodInvocation parameters", n);
    }
  }
  
  n->type = methodDecl->type;
};

void TypeChecker::dispatch(std::shared_ptr<ArrayAccess> n) {
  if (tmpExpression->type->arrayDepth < 1)
  {
    error("trying to access something that is not an array", n);
    return;
  }
  
  n->expression->accept(shared_from_this());
  
  if (!n->expression->type->equals(intNode))
  {
    error("array index in ArrayAccess must be integer", n);
  }
  
  n->type = std::make_shared<Type>(tmpExpression->type->basicType, tmpExpression->type->arrayDepth - 1);
  
  // REVIEW: is every array access an LValue?
  n->isLValue = true;
};

void TypeChecker::dispatch(std::shared_ptr<FieldAccess> n) {
  auto userType = dynamic_cast<UserType*>(tmpExpression->type->basicType.get());
  
  if (!userType)
  {
    error("field access on non-UserType", n);
  }
  
  auto classDecl = userType->declaration.lock();
  
  if (!classDecl)
  {
    fatalError("declaration missing for UserType in FieldAccess", n);
  }
  
  // check for field ID in class given by expression
  if (classDecl->fields.count(n->ID) != 1)
  {
    error("field access to unknown field (could not find field in UserType)", n);
  }
  
  auto fieldDecl = classDecl->fields[n->ID].lock();
  
  n->type = fieldDecl->type;
  
  // REVIEW: is every field access an LValue?
  n->isLValue = true;
};

void TypeChecker::dispatch(std::shared_ptr<AssignmentExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  // Check left side for lvalueness
  if (!n->expression1->isLValue) {
    error("required lvalue for expression " + Checker::printNode(n->expression1) + " in", n);
  }
  
  // check if left and right expression have same type
  if (!n->expression1->type->equals(n->expression2->type))
  {
    error("assignment type mismatch in " + Checker::printNode(n->expression2) + " in ", n);
  }
  
  n->type = n->expression2->type;
};

void TypeChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(booleanNode) ||
      !n->expression2->type->equals(booleanNode)) {
    error("type mismatch in Or expression", n);
  }
  
  n->type = booleanNode;
};

void TypeChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(booleanNode) ||
      !n->expression2->type->equals(booleanNode)) {
    error("type mismatch in And expression", n);
  }
  
  n->type = booleanNode;
};

void TypeChecker::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(n->expression2->type)) {
    error("type mismatch in equality expression", n);
  }
  
  n->type = booleanNode;
};

void TypeChecker::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(intNode) ||
      !n->expression2->type->equals(intNode)) {
    error("type mismatch in relational expression", n);
  }
  
  n->type = booleanNode;
};

void TypeChecker::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(intNode) ||
      !n->expression2->type->equals(intNode)) {
    error("type mismatch in additive expression", n);
  }
  
  n->type = intNode;
  n->isLValue = false;
};

void TypeChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(intNode) ||
      !n->expression2->type->equals(intNode)) {
    error("type mismatch in multiplicative expression", n);
  }
  
 n->type = intNode;
  n->isLValue = false;
};

void TypeChecker::dispatch(std::shared_ptr<CallExpression> n) {
  auto decl = n->declaration.lock();
  
  if (!decl) {
    fatalError("declaration on CallExpression has to be set before type analysis", n);
  }
  
  // Check number of arguments equals number of parameters
  if (n->arguments.size() != decl->parameters.size()) {
    error("number of arguments CallExpression <-> Method does not match");
  }
  
  for (int i = 0; i < n->arguments.size(); i++) {
    n->arguments[i]->accept(shared_from_this());
    
    if (!n->arguments[i]->type->equals(decl->parameters[i]->type)) {
      error("type mismatch of CallExpression parameters");
    }
  }
  
  n->type = decl->type;
};

void TypeChecker::dispatch(std::shared_ptr<UnaryLeftExpression> n) {
  n->expression->accept(shared_from_this());
  n->op->accept(shared_from_this());
  
  // expression must be boolean
  if (!n->expression->type->equals(n->op->type))
  {
    error("type mismatch in UnaryLeftExpression", n);
  }
  
  n->type = n->op->type;
};

void TypeChecker::dispatch(std::shared_ptr<UnaryRightExpression> n) {
  n->expression->accept(shared_from_this());
  
  // handle checks in operator
  tmpExpression = n->expression;
  n->op->accept(shared_from_this());
  
  n->type = n->op->type;
  n->isLValue = n->op->isLValue;
};

void TypeChecker::dispatch(std::shared_ptr<CNull> n) {
  // TODO: add singleton for special UserType nulltpr?
};

void TypeChecker::dispatch(std::shared_ptr<CThis> n) {
  auto decl = n->declaration.lock();
  
  TypedNode* typedNode;
  
  if (!decl || !(typedNode = dynamic_cast<TypedNode*>(decl.get())))
  {
    fatalError("declaration on CThis is missing or not TypedNode", n);
  }
  
  if (!typedNode->type || !typedNode->type->basicType)
  {
    fatalError("CThis points to declaration with missing type or missing basicType", n);
  }
  
  n->type = typedNode->type;
};

void TypeChecker::dispatch(std::shared_ptr<CTrue> n) {
  n->type = booleanNode;
};

void TypeChecker::dispatch(std::shared_ptr<CFalse> n) {
  n->type = booleanNode;
};

void TypeChecker::dispatch(std::shared_ptr<CRef> n) {
  // type of CRef is type of declaration
  
  // static cast because we know every CRef is an Expression
  // necessary to access fields of base class TypedNode
  Expression* expr = static_cast<Expression*>(n.get());
  
  auto decl = n->declaration.lock();
  TypedNode* typedNode;
  
  if (!decl || !(typedNode = dynamic_cast<TypedNode*>(decl.get())))
  {
    fatalError("declaration on CRef is missing or not TypedNode", n);
  }
  
  if (!typedNode->type || !typedNode->type->basicType)
  {
    fatalError("CRef points to declaration with missing or incomplete type", n);
  }
  
  expr->type = std::make_shared<Type>(typedNode->type->basicType, typedNode->type->arrayDepth);
  expr->isLValue = typedNode->isLValue;
}

void TypeChecker::dispatch(std::shared_ptr<CIntegerLiteral> n) {
  // check if value in string is correct int
  try
  {
    n->value = std::stoi(n->integer);
  }
  catch (std::invalid_argument &e)
  {
    error("CIntegerLiteral can't be converted to 32bit integer", n);
  }
  
  n->type = intNode;
};

void TypeChecker::dispatch(std::shared_ptr<NewObject> n) {
  // static cast because we know every NewObject is an Expression
  // necessary to access fields of base class TypedNode
  Expression* expr = static_cast<Expression*>(n.get());
  
  if (!n->userType)
  {
    fatalError("fatal error: missing userType", n);
  }
  
  expr->type = std::make_shared<Type>(n->userType, 0);
};

void TypeChecker::dispatch(std::shared_ptr<NewArray> n) {
  n->expression->accept(shared_from_this());
  // type already set by parser
  
  if (!n->expression->type->equals(intNode))
  {
    error("array size in NewArray must be integer", n);
  }
};

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

void TypeChecker::dispatch(std::shared_ptr<Negate> n) {
  n->type = booleanNode;
};

void TypeChecker::dispatch(std::shared_ptr<Minus> n) {
  // expression must be int
  n->type = intNode;
};
