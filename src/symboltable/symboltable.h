#pragma once

#include <map>
#include <string>
#include <memory>

#include "token.h"

namespace cmpl
{
  
  struct Declaration {
    std::string foo;
  };
  
  class Scope {
    public:
      std::unique_ptr<std::map<StringIdentifier, std::weak_ptr<Declaration> > > declarations;
      std::shared_ptr<Scope> parent;
      Scope(std::shared_ptr<Scope> p);
      std::weak_ptr<Declaration> lookup(StringIdentifier name);
      void insert(StringIdentifier name, std::weak_ptr<Declaration> decl);
  };
  
  
  class SymbolTable
  {
    public:
      void enterScope();
      void leaveScope();
      void insert(StringIdentifier name, std::weak_ptr<Declaration> decl);
      std::weak_ptr<Declaration> lookup(StringIdentifier name);
      bool isDefinedInCurrentScope(StringIdentifier name);
      SymbolTable() : current(nullptr) {}
      
    private:
      std::shared_ptr<Scope> current;
  };

}