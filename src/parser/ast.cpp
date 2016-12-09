#include "../stringtable/stringtable.h"
#include "ast.h"

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <assert.h>
#include <libfirm/firm.h>

#include "types.h"

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

void StaticLibraryCallExpression::accept(std::shared_ptr<Dispatcher> d) {
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






#pragma mark - equals

bool Type::equals(std::shared_ptr<Type> t) {
  assert(t != nullptr);
  assert(t->basicType != nullptr);
  
  return shared_from_this()->arrayDepth == t->arrayDepth && shared_from_this()->basicType->equals(t->basicType);
}

/*
bool BasicType::equals(std::shared_ptr<BasicType> t)
{
  return (this == t.get());
}
*/

const std::shared_ptr<TypeInt> TypeInt::instance = std::shared_ptr<TypeInt>(new TypeInt());
const std::shared_ptr<TypeBoolean> TypeBoolean::instance = std::shared_ptr<TypeBoolean>(new TypeBoolean());
const std::shared_ptr<TypeVoid> TypeVoid::instance = std::shared_ptr<TypeVoid>(new TypeVoid());
const std::shared_ptr<FakeType> FakeType::instance = std::shared_ptr<FakeType>(new FakeType());
const std::shared_ptr<NullType> NullType::instance = std::shared_ptr<NullType>(new NullType());


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




#pragma mark - Misc

bool Expression::isValidSemanticType() { // Semantic types type expressions, expressions cannot be void
  if (dynamic_cast<TypeVoid*>(type->basicType.get())) {
    return true;
  } else {
    return false;
  }
}


Type::Type(std::shared_ptr<BasicType> const& basicType, int const& arrayDepth) : basicType(basicType), arrayDepth(arrayDepth) {
  // Null with arrayDepth > 0 must not exist
  assert(basicType != nullptr);
  assert(!dynamic_cast<NullType*>(basicType.get()) || arrayDepth == 0);
};






#pragma mark - Firm types

ir_type * Type::getFirmType() {
  ir_type *elem_type = basicType->getFirmType();
  
  if (arrayDepth <= 0) {
    // scalar type
    return elem_type;
  }
  
  ir_type *my_type = elem_type;
  int i = arrayDepth;
  while (i > 0) {
    // pointer to array of (scalars) or (pointers to arrays of ...)
    my_type = new_type_pointer(new_type_array(my_type, 0));
    i--;
  }
  
  return my_type;
}

ir_type * UserType::getFirmType() {
  auto d = declaration.lock();
  assert(d);
  
  return new_type_pointer(d->declared_type);
}

ir_type * FakeType::getFirmType() {
  return NULL; // ?
}

ir_type * TypeBoolean::getFirmType() {
  static ir_type *boo_type;
  if(!boo_type)
  {
    boo_type = new_type_primitive(mode_Bu);
  }
  return boo_type;
}

ir_type * TypeInt::getFirmType() {
  static ir_type *int_type;
  if(!int_type)
  {
	  int_type = new_type_primitive(mode_Is);
  }
  return int_type;
}

ir_type * TypeVoid::getFirmType() {
  return NULL;
}

ir_type * NullType::getFirmType() {
  return NULL;
}











static ir_type *print_type = NULL;

ir_type *StaticLibraryCallExpression::getFirmType() {
  if (!print_type) {
    print_type = new_type_method(1, 0, false, cc_cdecl_set, mtp_no_property);
    set_method_param_type(print_type, 0, Types::getIntNode()->getFirmType());
  }
  
  return print_type;
}

#ifdef __APPLE__
#define PRINTLN_NAME "_println"
#else
#define PRINTLN_NAME "println"
#endif

static ir_entity *print_entity = NULL;

ir_entity *StaticLibraryCallExpression::getFirmEntity() {
  if (!print_entity) {
    print_entity = new_global_entity(get_glob_type(), new_id_from_str(PRINTLN_NAME),
                                    getFirmType(), ir_visibility_external,
                                    IR_LINKAGE_DEFAULT);
  }
  
  return print_entity;
}
