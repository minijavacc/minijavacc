#pragma once

#include <map>
#include <string>
#include <memory>

namespace cmpl
{
  
  struct Declaration {
    std::string foo;
  };
  
  class Scope {
    public:
      std::unique_ptr<std::map<std::string, std::weak_ptr<Declaration> > > declarations;
      std::shared_ptr<Scope> parent;
      Scope(std::shared_ptr<Scope> p);
      std::weak_ptr<Declaration> lookup(std::string name);
      void insert(std::string name, std::weak_ptr<Declaration> decl);
  };
  
  
  class SymbolTable
  {
    public:
      void enterScope();
      void leaveScope();
      void insert(std::string name, std::weak_ptr<Declaration> decl);
      std::weak_ptr<Declaration> lookup(std::string name);
      bool isDefinedInCurrentScope(std::string name);
      SymbolTable() : current(nullptr) {}
      
    private:
      std::shared_ptr<Scope> current;
  };

}