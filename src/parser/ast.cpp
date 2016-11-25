#include "../stringtable/stringtable.h"
#include "ast.h"

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <assert.h>
#include <libfirm/firm.h>

using namespace cmpl;
  

    





void TypeBoolean::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void TypeVoid::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void UserType::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void TypeInt::accept (std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Type::accept (std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void NotEquals::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Equals::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void LessThan::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void LessThanOrEqual::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void GreaterThan::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void GreaterThanOrEqual::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Add::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Subtract::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Multiply::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Divide::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Modulo::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Negate::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Minus::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void MethodInvocation::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void FieldAccess::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void ArrayAccess::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void AssignmentExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void LogicalOrExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void LogicalAndExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void EqualityExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void RelationalExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void AdditiveExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void MultiplicativeExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void CallExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void UnaryLeftExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void UnaryRightExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void CNull::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void CFalse::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void CTrue::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void CThis::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void CIntegerLiteral::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void CRef::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void NewObject::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void NewArray::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Parameter::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Block::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void IfStatement::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void IfElseStatement::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void ExpressionStatement::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void WhileStatement::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void ReturnExpressionStatement::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void ReturnStatement::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void EmptyStatement::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void LocalVariableDeclaration::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void LocalVariableExpressionDeclaration::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Field::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Method::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void MainMethod::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void ClassDeclaration::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void Program::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void NullType::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}





// firm types

ir_type * UserType::getFirmType() {
  auto d = declaration.lock();
  assert(d);
  
  return d->declared_type;
}

ir_type * FakeType::getFirmType() {
  return NULL; // ?
}

ir_type * TypeBoolean::getFirmType() {
  static ir_type *int_type = new_type_primitive(mode_Bu);
  return int_type;
}

ir_type * TypeInt::getFirmType() {
  static ir_type *int_type = new_type_primitive(mode_Bu);
  return int_type;
}

ir_type * TypeVoid::getFirmType() {
  return NULL;
}

ir_type * NullType::getFirmType() {
  return NULL;
}




// equals

bool Type::equals(std::shared_ptr<Type> t) {
  assert(t != nullptr);
  assert(t->basicType != nullptr);
  
  return shared_from_this()->arrayDepth == t->arrayDepth && shared_from_this()->basicType->equals(t->basicType);
}

bool TypeInt::equals(std::shared_ptr<BasicType> t) {
  if (dynamic_cast<TypeInt*>(t.get())) {
    return true;
  } else {
    return false;
  }
}

bool TypeBoolean::equals(std::shared_ptr<BasicType> t) {
  if (dynamic_cast<TypeBoolean*>(t.get())) {
    return true;
  } else {
    return false;
  }
}

bool TypeVoid::equals(std::shared_ptr<BasicType> t) {
  if (dynamic_cast<TypeVoid*>(t.get())) {
    return true;
  } else {
    return false;
  }
}

void FakeType::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

bool FakeType::equals(std::shared_ptr<BasicType> other) {
  return false;
}

bool UserType::equals(std::shared_ptr<BasicType> other) {
  if (UserType* ut = dynamic_cast<UserType*>(other.get())) {
    return ID == ut->ID;
  } else {
    return false;
  }
}

bool NullType::equals(std::shared_ptr<BasicType> other) {
  return false;
}




// misc

void Expression::setDefinition(void* irn) {};

void Expression::assign(ir_node* n) {};

bool Expression::isValidSemanticType() { // Semantic types type expressions, expressions cannot be void
  if (dynamic_cast<TypeVoid*>(type->basicType.get())) {
    return true;
  } else {
    return false;
  }
}

void CRef::assign(ir_node* irn) {
  auto ptr = declaration.lock();
  if (ptr) {
    ptr->firm_node = irn;
  }
}

void Parameter::assign(ir_node* n) {
  firm_node = n;
}

void LocalVariableDeclaration::assign(ir_node* n) {
  firm_node = n;
}

void LocalVariableExpressionDeclaration::assign(ir_node* n) {
  firm_node = n;
}

void Field::assign(ir_node* n) {
  // store node in ent
  //        ir_node *addr = new_Address(ent);
  //        ir_node *st = new_Store(get_store(), addr, n, this->type->firm_type, cons_none);
  //        ir_node *m = new_Proj(st, mode_M, pn_Store_M);
  //        set_store(m);
}



Type::Type(std::shared_ptr<BasicType> const& basicType, int const& arrayDepth) : basicType(std::move(basicType)), arrayDepth(arrayDepth) {
  // Null with arrayDepth > 0 must not exist
  assert(!dynamic_cast<NullType*>(basicType.get()) || arrayDepth == 0);
};


