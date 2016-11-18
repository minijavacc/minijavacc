#include "symboltable.h"

#include <map>
#include <string>
#include <memory>
#include <iostream>





using namespace cmpl;



// --- Scope ---

std::weak_ptr<Declaration> Scope::lookup(StringIdentifier name) {
  if (declarations->count(name) > 0) {
    return declarations->at(name);
  } else {
    if (parent != nullptr) {
      return parent->lookup(name);
    }
    
    return std::weak_ptr<Declaration>();
  }
}

Scope::Scope(std::shared_ptr<Scope> p) : parent(p), 
                                         declarations(new std::map<StringIdentifier, std::weak_ptr<Declaration> >()) { }

void Scope::insert(StringIdentifier name, std::weak_ptr<Declaration> decl) {
  (*declarations)[name] = decl;
}




// --- SymbolTable ---

void SymbolTable::enterScope() {
  current = std::unique_ptr<Scope>(new Scope(current));
}

void SymbolTable::leaveScope() {
  if (current) {
    current = current->parent;
  }
}

std::weak_ptr<Declaration> SymbolTable::lookup(StringIdentifier name) {
  if (current) {
    return current->lookup(name);
  } else {
    return std::weak_ptr<Declaration>();
  }
}

bool SymbolTable::isDefinedInCurrentScope(StringIdentifier name) {
  return current->declarations->count(name) > 0;
}

void SymbolTable::insert(StringIdentifier name, std::weak_ptr<Declaration> decl) {
  if (current) {
    current->insert(name, decl);
  }
}