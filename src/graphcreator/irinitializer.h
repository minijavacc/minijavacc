#pragma once

#include "../structures/ast.h"
#include "creator.h"
#include "../backend/amd64instructions.h"

#include <libfirm/firm.h>

namespace cmpl {

  class IRInitializer : public Dispatcher, public std::enable_shared_from_this<IRInitializer> {
  private:
    void error(const std::string &err);
    std::shared_ptr<ClassDeclaration> currentClassDeclaration;
    std::shared_ptr<Method> currentMethod;
  public:
    void dispatch(std::shared_ptr<Program> n) override;
    void dispatch(std::shared_ptr<ClassDeclaration> n) override;
    void dispatch(std::shared_ptr<MainMethod> n) override;
    void dispatch(std::shared_ptr<Method> n) override;
    void dispatch(std::shared_ptr<Field> n) override;
  };
  
  class IRInitializerError : public CreatorError 
  {
    public:
      IRInitializerError(const char* err) : CreatorError(err) { }
  };

}
