#include "irinitializer.h"
#include "stringtable.h"


using namespace cmpl;


inline void IRInitializer::error(const std::string &err)
{
  throw IRInitializerError(("IRInitializer: " + err).c_str());
}

void IRInitializer::dispatch(std::shared_ptr<Program> n) {
  // Initialize declared_type first
  for (auto const& d : n->classDeclarations) {
    std::string str = StringTable::lookupIdentifier(d->ID);
    d->declared_type = new_type_class(new_id_from_str(str.c_str()));
  }
  
  for (auto const& d : n->classDeclarations) {
    d->accept(shared_from_this());
  }
};

void IRInitializer::dispatch(std::shared_ptr<ClassDeclaration> n) {
  currentClassDeclaration = n;
  
  unsigned int fieldOffset = 0;
  
  // Fields first (important)
  for (auto const &p : n->fields) {
    auto f = p.second.lock();
    assert(f);
    
    // Propagate type
    ir_type *t = f->type->getFirmType();
    assert(t);
    
    // Build entity
    // simple name mangling similar to C++
    std::string fieldName = StringTable::lookupIdentifier(f->ID);
    std::string className = StringTable::lookupIdentifier(currentClassDeclaration->ID);
    std::string mangledName = "_ZF" + std::to_string(className.length()) + className + std::to_string(fieldName.length()) + fieldName + "E";
    //                           ^---- F = Field
    
    ir_entity *e = new_entity(currentClassDeclaration->declared_type, new_id_from_str(mangledName.c_str()), t);
    assert(e);
    
    // set field offset
    set_entity_offset(e, fieldOffset);
    fieldOffset += get_type_size(t);
    
    f->firm_type = t;
    f->firm_entity = e;
  }
  
  // set class size
  unsigned int classSize = get_type_size(n->declared_type) + fieldOffset;
  set_type_size(n->declared_type, classSize);
  
  // set class state
  set_type_state(n->declared_type, layout_fixed);
  
  
  // Methods last (important)
  for (auto const &p : n->methods) {
    auto m = p.second.lock();
    assert(m);
    m->accept(shared_from_this());
  }
  
  // Main method
  for (auto const &m : n->classMembers) {
    if (dynamic_cast<MainMethod*>(m.get())) {
      m->accept(shared_from_this());
    }
  }
};

void IRInitializer::dispatch(std::shared_ptr<MainMethod> n) {
  std::string name = StringTable::lookupIdentifier(n->ID);
  ir_type *t = new_type_method(0, 1, false, cc_cdecl_set, mtp_no_property);
  assert(t);
  set_method_res_type(t, 0, Types::getIntNode()->getFirmType());
  ir_entity *e = new_entity(currentClassDeclaration->declared_type, new_id_from_str((std::string(AMD64LdNamePrefix) + std::string("main")).c_str()), t);
  assert(e);
  ir_graph *g = new_ir_graph(e, (int) n->localVariables.size());
  assert(g);
  
  n->declared_type = t;
  n->firm_entity = e;
  n->firm_graph = g;
  
  // Set parameter indices for local variables
  int i = 0;
  for (auto const& d : n->localVariables) {
    d->parameterIndex = i++;
  }
};

void IRInitializer::dispatch(std::shared_ptr<Method> n) {
  std::string name = StringTable::lookupIdentifier(n->ID);
  int nret = 1;
  if (dynamic_cast<TypeVoid*>(n->type->basicType.get())) {
    nret = 0;
  }
  
  // Build type
  ir_type *t = new_type_method(1 + n->parameters.size(), nret, false, cc_cdecl_set, mtp_no_property);
  assert(t);
  
  // Set return type
  if (nret > 0) {
    set_method_res_type(t, 0, n->type->getFirmType());
  }
  
  // Set this pointer type
  ir_type *th = new_type_pointer(currentClassDeclaration->declared_type);
  assert(th);
  set_method_param_type(t, 0, th);
  
  // Set other parameter types
  int i = 1;
  for (auto const& p : n->parameters) {
    p->accept(shared_from_this());
    set_method_param_type(t, i, p->type->getFirmType());
    p->parameterIndex = i;
    i++;
  }
  
  // Set parameter indices for local variables
  for (auto const& d : n->localVariables) {
    d->parameterIndex = i++;
  }
  
  // Build entity
  // simple name mangling similar to C++
  std::string methodName = StringTable::lookupIdentifier(n->ID);
  std::string className = StringTable::lookupIdentifier(currentClassDeclaration->ID);
  std::string mangledName = "_ZM" + std::to_string(className.length()) + className + std::to_string(methodName.length()) + methodName + "E";
  //                           ^---- M = Method
  ir_entity *e = new_entity(currentClassDeclaration->declared_type, new_id_from_str(mangledName.c_str()), t);
  
  // Build graph
  int num = (int) (1 + n->parameters.size() + n->localVariables.size());
  ir_graph *g = new_ir_graph(e, num);
  assert(g);
  
  n->declared_type = t;
  n->firm_entity = e;
  n->firm_graph = g;
};

void IRInitializer::dispatch(std::shared_ptr<Field> n) {};

