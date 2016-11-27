#pragma once

#include "ast.h"
#include <map>

namespace cmpl
{
  /*
   * short explanation about types
   * =============================
   * - AST-nodes with a type inherit from TypedNode
   * - a Type consists of a BasicType an a arrayDepth
   * - each BasicType only exists once, except the UserType, which exists once for each ID
   */
   
  // singletons for BasicTypes
  const std::shared_ptr<BasicType> voidBasicNode    = std::make_shared<TypeVoid>();
  const std::shared_ptr<BasicType> intBasicNode     = std::make_shared<TypeInt>();
  const std::shared_ptr<BasicType> fakeBasicNode    = std::make_shared<FakeType>();
  const std::shared_ptr<BasicType> booleanBasicNode = std::make_shared<TypeBoolean>();
  const std::shared_ptr<BasicType> nullBasicNode    = std::make_shared<NullType>();
  
  // singletons for simple Types (arrayDepth = 0)
  // not used as singletons yet, only for comparing types
  const std::shared_ptr<Type> voidNode    = std::make_shared<Type>(voidBasicNode, 0);
  const std::shared_ptr<Type> intNode     = std::make_shared<Type>(intBasicNode, 0);
  const std::shared_ptr<Type> fakeType    = std::make_shared<Type>(fakeBasicNode, 0);
  const std::shared_ptr<Type> booleanNode = std::make_shared<Type>(booleanBasicNode, 0);
  const std::shared_ptr<Type> nullNode    = std::make_shared<Type>(nullBasicNode, 0);
  
  class Types
  {
  public: 
    static std::shared_ptr<UserType> getUserTypeBasicNode(StringIdentifier ID);
  
  private:
    static std::map<StringIdentifier, std::shared_ptr<UserType> > userTypesMap;
  };
}
