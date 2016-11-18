#pragma once

#include <map>
#include <string>
#include <memory>

#include "node.h"

namespace cmpl
{
  
  class Scope {
    public:
      std::unique_ptr<std::map<StringIdentifier, std::weak_ptr<Node> > > declarations;
      std::shared_ptr<Scope> parent;
      Scope(std::shared_ptr<Scope> p);
      std::weak_ptr<Node> lookup(StringIdentifier name);
      void insert(StringIdentifier name, std::weak_ptr<Node> decl);
  };
  
  
  class SymbolTable
  {
    public:
      void enterScope();
      void leaveScope();
      void insert(StringIdentifier name, std::weak_ptr<Node> decl);
      std::weak_ptr<Node> lookup(StringIdentifier name);
      bool isDefinedInCurrentScope(StringIdentifier name);
      SymbolTable() : current(nullptr) {}
      
    private:
      std::shared_ptr<Scope> current;
  };

}