#include "../stringtable/stringtable.h"
#include "ast.h"
#include "amd64instructions.h"

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

void SLCPrintlnExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void SLCWriteExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void SLCFlushExpression::accept(std::shared_ptr<Dispatcher> d) {
  d->dispatch(shared_from_this());
}

void SLCReadExpression::accept(std::shared_ptr<Dispatcher> d) {
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
  if (!firm_type)
  {
    firm_type = basicType->getFirmType();
    
    if (arrayDepth <= 0) {
      // scalar type
      return firm_type;
    }
    
    int i = arrayDepth;
    while (i > 0) {
      // pointer to array of (scalars) or (pointers to arrays of ...)
      firm_type = new_type_pointer(new_type_array(firm_type, 0));
      i--;
    }
  }
  
  return firm_type;
}

ir_type * UserType::getFirmType() {
  if (!firm_type)
  {
    auto d = declaration.lock();
    assert(d);
  
    firm_type =  new_type_pointer(d->declared_type);
  }
  
  return firm_type;
}

ir_type * FakeType::getFirmType() {
  return NULL; // ?
}

ir_type * TypeBoolean::getFirmType() {
  if(!firm_type)
  {
    firm_type = new_type_primitive(mode_Bu);
  }
  
  return firm_type;
}

ir_type * TypeInt::getFirmType() {
  if(!firm_type)
  {
	  firm_type = new_type_primitive(mode_Is);
  }
  
  return firm_type;
}

ir_type * TypeVoid::getFirmType() {
  return NULL;
}

ir_type * NullType::getFirmType() {
  return NULL;
}


static ir_type *void_takes_int_type = NULL;
inline ir_type *assureVoidTakesInt() {
  if(!void_takes_int_type) {
    void_takes_int_type = new_type_method(1, 0, false, cc_cdecl_set, mtp_no_property);
    set_method_param_type(void_takes_int_type, 0, Types::getIntNode()->getFirmType());
  }
  return void_takes_int_type;
}

ir_type *SLCPrintlnExpression::getFirmType() {
  return assureVoidTakesInt();
}
ir_type *SLCWriteExpression::getFirmType() {
  return assureVoidTakesInt();
}

static ir_type *void_type = new_type_method(0, 0, false, cc_cdecl_set, mtp_no_property);
ir_type *SLCFlushExpression::getFirmType() {
  return void_type;
}

static ir_type *int_type = NULL;
ir_type *SLCReadExpression::getFirmType() {
  if(!int_type) {
    int_type = new_type_method(0, 1, false, cc_cdecl_set, mtp_no_property);
    set_method_res_type(int_type, 0, Types::getIntNode()->getFirmType());
  }
  
  return int_type;
}
/*
static ir_entity *print_entity = NULL;

ir_entity *StaticLibraryCallExpression::getFirmEntity() {
  if (!print_entity) {
    print_entity = new_global_entity(get_glob_type(), new_id_from_str((std::string(AMD64LdNamePrefix) + std::string("println")).c_str()),
                                    getFirmType(), ir_visibility_external,
                                    IR_LINKAGE_DEFAULT);
  }
  
  return print_entity;
}
*/
static ir_entity *print_entity_println = NULL;
static ir_entity *print_entity_write = NULL;
static ir_entity *print_entity_flush = NULL;
static ir_entity *print_entity_read  = NULL;

ir_entity *SLCPrintlnExpression::getFirmEntity() {
  if(!print_entity_println) {
    print_entity_println = new_global_entity(get_glob_type(), new_id_from_str((std::string(AMD64LdNamePrefix) + std::string("println")).c_str()),
                                              getFirmType(), ir_visibility_external, IR_LINKAGE_DEFAULT);
  }
  return print_entity_println;
}

ir_entity *SLCWriteExpression::getFirmEntity() {
  if(!print_entity_write) {
    print_entity_write = new_global_entity(get_glob_type(), new_id_from_str((std::string(AMD64LdNamePrefix) + std::string("write")).c_str()),
                                            getFirmType(), ir_visibility_external, IR_LINKAGE_DEFAULT);
  }
  return print_entity_write;
}

ir_entity *SLCFlushExpression::getFirmEntity() {
  if(!print_entity_flush) {
    print_entity_flush = new_global_entity(get_glob_type(), new_id_from_str((std::string(AMD64LdNamePrefix) + std::string("flush")).c_str()),
                                            getFirmType(), ir_visibility_external, IR_LINKAGE_DEFAULT);
  }
  return print_entity_flush;
}
ir_entity *SLCReadExpression::getFirmEntity() {
  if(print_entity_read) {
    print_entity_read  = new_global_entity(get_glob_type(), new_id_from_str((std::string(AMD64LdNamePrefix) + std::string("read")).c_str()),
                                            getFirmType(), ir_visibility_external, IR_LINKAGE_DEFAULT);
  }
  return print_entity_read;
}
