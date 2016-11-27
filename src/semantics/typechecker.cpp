//
//  typechecker.cpp
//  mjcc
//
//  Created by Markus Schlegel on 18/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "typechecker.h"
#include "types.h"

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
void TypeChecker::dispatch(std::shared_ptr<NullType> n) { };
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
  if (!n->expression->type->equals(Types::getBooleanNode()))
  {
    error("expression in IfElseStatement must be of type boolean", n);
  }
};

void TypeChecker::dispatch(std::shared_ptr<ExpressionStatement> n) {
  n->expression->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<WhileStatement> n) {
  n->expression->accept(shared_from_this());
  
  // check if expression is boolean
  if (!n->expression->type->equals(Types::getBooleanNode()))
  {
    error("expression in WhileStatement must be of type boolean", n);
  }
  
  n->statement->accept(shared_from_this());
};

void TypeChecker::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  // type has been set by parser
};

void TypeChecker::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  // type has been set by parser
  // get type of expression
  n->expression->accept(shared_from_this());
  
  // check if type matches the type of the expression
  if (!n->type->equals(n->expression->type))
  {
    // check if Null is assigned to a UserType
    if (dynamic_cast<NullType*>(n->expression->type->basicType.get()) && 
        dynamic_cast<UserType*>(n->type->basicType.get()))
    {
      return;
    }
    
    // check if null is assigned to any array
    if (dynamic_cast<NullType*>(n->expression->type->basicType.get()) && 
        n->type->arrayDepth > 0)
    {
      return;
    }
    
    // no Null special case
    error("expression in LocalVariableExpressionDeclaration has wrong type", n);
  }
  
};

void TypeChecker::dispatch(std::shared_ptr<EmptyStatement> n) { };

void TypeChecker::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->expression->accept(shared_from_this());
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
  
  // check if expression is boolean
  if (!n->expression->type->equals(Types::getBooleanNode()))
  {
    error("expression in IfElseStatement must be of type boolean", n->expression);
  }
};

void TypeChecker::dispatch(std::shared_ptr<ReturnStatement> n) { };

void TypeChecker::dispatch(std::shared_ptr<ReturnExpressionStatement> n) {
  n->expression->accept(shared_from_this());
    //check first if its a void method, then return EXPR is invalid in java!
  if(currentMethod->type->equals(Types::getVoidNode()))
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
  
  assert(classDecl != nullptr);
  
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
      // check if Null is assigned to a UserType
      if (dynamic_cast<NullType*>(n->arguments[i]->type->basicType.get()) && 
          dynamic_cast<UserType*>(methodDecl->parameters[i]->type->basicType.get()))
      {
        continue;
      }
      
      // check if null is assigned to any array
      if (dynamic_cast<NullType*>(n->arguments[i]->type->basicType.get()) && 
          methodDecl->parameters[i]->type->arrayDepth > 0)
      {
        continue;
      }
      
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
  
  if (!n->expression->type->equals(Types::getIntNode()))
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
  
  assert(classDecl != nullptr);
  
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
  
  // check if one of both types is null
  // - only UserTypes and Arrays can be null
  if (dynamic_cast<NullType*>(n->expression2->type->basicType.get()))
  {
    if (n->expression1->type->arrayDepth == 0 && 
      !dynamic_cast<UserType*>(n->expression1->type->basicType.get()))
    {
      error("only Arrays and UserTypes can be assigned with null", n);
    }
  }
  // check if left and right expression have same type
  else if (!n->expression1->type->equals(n->expression2->type))
  {
    error("assignment type mismatch in " + Checker::printNode(n->expression2) + " in ", n);
  }
  
  n->type = n->expression2->type;
  return;
};

void TypeChecker::dispatch(std::shared_ptr<LogicalOrExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(Types::getBooleanNode()) ||
      !n->expression2->type->equals(Types::getBooleanNode())) {
    error("type mismatch in Or expression", n);
  }
  
  n->type = Types::getBooleanNode();
};

void TypeChecker::dispatch(std::shared_ptr<LogicalAndExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  assert(n->expression1->type != nullptr);
  assert(n->expression2->type != nullptr);
  
  if (!n->expression1->type->equals(Types::getBooleanNode()) ||
      !n->expression2->type->equals(Types::getBooleanNode())) {
    error("type mismatch in And expression", n);
  }
  
  n->type = Types::getBooleanNode();
};

void TypeChecker::dispatch(std::shared_ptr<EqualityExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  assert(n->expression1->type != nullptr);
  assert(n->expression2->type != nullptr);
  
  bool expr1IsNull = (dynamic_cast<NullType*>(n->expression1->type->basicType.get()) != nullptr);
  bool expr2IsNull = (dynamic_cast<NullType*>(n->expression2->type->basicType.get()) != nullptr);
  
  // check if one of both types is null
  if (expr1IsNull || expr2IsNull)
  {
    // - only UserTypes and Arrays can be null
    if ((expr1IsNull && 
      n->expression2->type->arrayDepth == 0 && 
      !dynamic_cast<UserType*>(n->expression2->type->basicType.get())) || 
      
      (expr2IsNull && 
      n->expression1->type->arrayDepth == 0 && 
      !dynamic_cast<UserType*>(n->expression1->type->basicType.get())))
    {
      error("only Arrays and UserTypes can be compared with null", n);
    }
  }
  // check if types are not equal
  else if (!n->expression1->type->equals(n->expression2->type)) {
    error("type mismatch in EqualityExpression", n);
  }
  
  n->type = Types::getBooleanNode();
  return;
};

void TypeChecker::dispatch(std::shared_ptr<RelationalExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(Types::getIntNode()) ||
      !n->expression2->type->equals(Types::getIntNode())) {
    error("type mismatch in relational expression", n);
  }
  
  n->type = Types::getBooleanNode();
};

void TypeChecker::dispatch(std::shared_ptr<AdditiveExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(Types::getIntNode()) ||
      !n->expression2->type->equals(Types::getIntNode())) {
    error("type mismatch in additive expression", n);
  }
  
  n->type = Types::getIntNode();
};

void TypeChecker::dispatch(std::shared_ptr<MultiplicativeExpression> n) {
  n->expression1->accept(shared_from_this());
  n->expression2->accept(shared_from_this());
  
  if (!n->expression1->type->equals(Types::getIntNode()) ||
      !n->expression2->type->equals(Types::getIntNode())) {
    error("type mismatch in multiplicative expression", n);
  }
  
  n->type = Types::getIntNode();
};

void TypeChecker::dispatch(std::shared_ptr<CallExpression> n) {
  auto decl = n->declaration.lock();
  
  assert(decl != nullptr);
  
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
  n->type = Types::getNullNode();
};

void TypeChecker::dispatch(std::shared_ptr<CThis> n) {
  auto decl = n->declaration.lock();
  
  TypedNode* typedNode;
  
  assert(decl != nullptr);
  
  typedNode = dynamic_cast<TypedNode*>(decl.get());
  
  assert(typedNode != nullptr);
  assert(typedNode->type != nullptr);
  assert(typedNode->type->basicType != nullptr);
  
  n->type = typedNode->type;
};

void TypeChecker::dispatch(std::shared_ptr<CTrue> n) {
  n->type = Types::getBooleanNode();
};

void TypeChecker::dispatch(std::shared_ptr<CFalse> n) {
  n->type = Types::getBooleanNode();
};

void TypeChecker::dispatch(std::shared_ptr<CRef> n) {
  // type of CRef is type of declaration
  auto decl = n->declaration.lock();
  TypedNode* typedNode;
  
  assert(decl != nullptr);
  
  typedNode = dynamic_cast<TypedNode*>(decl.get());
  
  assert(typedNode != nullptr);
  assert(typedNode->type != nullptr);
  assert(typedNode->type->basicType != nullptr);
  
  n->type = typedNode->type;
  n->isLValue = true;
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
  catch (std::out_of_range &e)
  {
    error("CIntegerLiteral can't be converted to 32bit integer", n);
  }
  
  n->type = Types::getIntNode();
};

void TypeChecker::dispatch(std::shared_ptr<NewObject> n) {
  // static cast because we know every NewObject is an Expression
  // necessary to access fields of base class TypedNode
  Expression* expr = static_cast<Expression*>(n.get());
  
  assert(n->userType != nullptr);
  
  expr->type = std::make_shared<Type>(n->userType, 0);
};

void TypeChecker::dispatch(std::shared_ptr<NewArray> n) {
  n->expression->accept(shared_from_this());
  // type already set by parser
  
  if (n->type->basicType->equals(Types::getVoidNode()->basicType))
  {
    error("creation of void array not allowed", n);
  }
  
  if (!n->expression->type->equals(Types::getIntNode()))
  {
    error("array size in NewArray must be integer", n);
  }
};

void TypeChecker::dispatch(std::shared_ptr<StaticLibraryCallExpression> n) {
  n->expression->accept(shared_from_this());
  
  if (!n->expression->type->equals(Types::getIntNode()))
  {
    error("library call parameter must be integer", n);
  }
  
  n->type = Types::getVoidNode();
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
  n->type = Types::getBooleanNode();
};

void TypeChecker::dispatch(std::shared_ptr<Minus> n) {
  // expression must be int
  n->type = Types::getIntNode();
};
