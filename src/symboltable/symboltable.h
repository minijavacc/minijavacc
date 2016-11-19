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
      bool lookup(StringIdentifier name, std::weak_ptr<Node> &res);
      void insert(StringIdentifier name, std::weak_ptr<Node> decl);
  };
  
  
  class SymbolTable
  {
    public:
      void enterScope();
      void leaveScope();
      void insert(StringIdentifier name, std::weak_ptr<Node> decl);
      bool lookup(StringIdentifier name, std::weak_ptr<Node> &res);
      bool hasValueFor(StringIdentifier name);
      SymbolTable() : current(nullptr) {}
      
    private:
      std::shared_ptr<Scope> current;
  };

}
