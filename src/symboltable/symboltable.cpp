#include "symboltable.h"

#include <map>
#include <string>
#include <memory>
#include <iostream>





using namespace cmpl;



// --- Scope ---

bool Scope::lookup(StringIdentifier name, std::weak_ptr<Node> &res) {
  if (declarations->count(name) > 0) {
    res = declarations->at(name);
    return true;
  } else {
    if (parent != nullptr) {
      return parent->lookup(name, res);
    }
    
    return false;
  }
}

Scope::Scope(std::shared_ptr<Scope> p) : parent(p), 
                                         declarations(new std::map<StringIdentifier, std::weak_ptr<Node> >()) { }

void Scope::insert(StringIdentifier name, std::weak_ptr<Node> decl) {
  (*declarations)[name] = decl;
}




// --- SymbolTable ---

void SymbolTable::enterScope() {
  current = std::shared_ptr<Scope>(new Scope(current));
}

void SymbolTable::leaveScope() {
  if (current) {
    current = current->parent;
  } else {
    std::cerr << "fatal: trying to leave scope, but already on top level" << std::endl;
  }
}

bool SymbolTable::lookup(StringIdentifier name, std::weak_ptr<Node> &res) {
  if (current) {
    return current->lookup(name, res);
  } else {
    return false;
  }
}

void SymbolTable::insert(StringIdentifier name, std::weak_ptr<Node> decl) {
  if (current) {
    current->insert(name, decl);
  } else {
    std::cerr << "fatal: trying to insert " + StringTable::lookupIdentifier(name) + " into scope, but there is no scope." << std::endl;
  }
}

bool SymbolTable::hasValueFor(StringIdentifier name) {
  std::weak_ptr<Node> ptr;
  return lookup(name, ptr);
}
