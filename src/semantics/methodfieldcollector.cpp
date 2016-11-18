//
//  methodfieldcollector.cpp
//  mjcc
//
//  Created by Markus Schlegel on 17/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "methodfieldcollector.h"

#include <iostream>

using namespace cmpl;



void MethodFieldCollector::dispatch(std::shared_ptr<Program> n) {
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
  }
};

void MethodFieldCollector::dispatch(std::shared_ptr<ClassDeclaration> n) {
  for (auto const& c : n->classMembers) {
    currentClassDeclaration = n;
    c->accept(shared_from_this());
  }
  
  std::cout << "num fields: " << n->fields.size();
  std::cout << "num methods: " << n->methods.size();
};

void MethodFieldCollector::dispatch(std::shared_ptr<Field> n) {
  currentClassDeclaration->fields.emplace(n->ID, n);
};

void MethodFieldCollector::dispatch(std::shared_ptr<Method> n) {
  currentClassDeclaration->methods.emplace(n->ID, n);
};

void MethodFieldCollector::dispatch(std::shared_ptr<MainMethod> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Type> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<UserType> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<TypeInt> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<TypeBoolean> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<TypeVoid> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Parameter> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Block> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<IfStatement> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<WhileStatement> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<LocalVariableDeclaration> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<EmptyStatement> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<IfElseStatement> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<ReturnStatement> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<ReturnExpressionStatement> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<MethodInvocation> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<ArrayAccess> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<FieldAccess> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<EqualityExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<RelationalExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<CallExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<CNull> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<CThis> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<CTrue> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<CFalse> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<CRef> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<NewObject> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<NewArray> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Equals> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<NotEquals> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<LessThan> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<GreaterThan> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Add> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Subtract> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Multiply> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Divide> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Modulo> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Negate> n) { };
void MethodFieldCollector::dispatch(std::shared_ptr<Minus> n) { };
