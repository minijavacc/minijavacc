#pragma once

class Types;

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
   
  // all BasicTypes are Singletons! access via <BasicType>::instance
  
  // singletons for simple Types (arrayDepth = 0)
  // not used as singletons yet, only for comparing types
  /*
  std::shared_ptr<Type> voidNode    = std::make_shared<Type>(TypeVoid::instance, 0);
  std::shared_ptr<Type> intNode     = std::make_shared<Type>(TypeInt::instance, 0);
  std::shared_ptr<Type> fakeNode    = std::make_shared<Type>(FakeType::instance, 0);
  std::shared_ptr<Type> booleanNode = std::make_shared<Type>(TypeBoolean::instance, 0);
  std::shared_ptr<Type> nullNode    = std::make_shared<Type>(NullType::instance, 0);
  */
  
  class Types
  {
  public: 
    static std::shared_ptr<Type> getVoidNode();
    static std::shared_ptr<Type> getIntNode();
    static std::shared_ptr<Type> getFakeNode();
    static std::shared_ptr<Type> getBooleanNode();
    static std::shared_ptr<Type> getNullNode();
    
    static std::shared_ptr<UserType> getUserTypeBasicNode(StringIdentifier ID);
    
  private:
    static std::map<StringIdentifier, std::shared_ptr<UserType> > userTypesMap;
  };
}
