#include "types.h"

namespace cmpl
{
  std::map<StringIdentifier, std::shared_ptr<UserType> > Types::userTypesMap;
  
  std::shared_ptr<UserType> Types::getUserTypeBasicNode(StringIdentifier ID)
  {
    if (userTypesMap.count(ID) > 0)
    {
      return userTypesMap[ID];
    }
    else
    {
      std::shared_ptr<UserType> u = std::make_shared<UserType>(ID);
      userTypesMap.emplace(ID, u);
      return u;
    }
  }
  
  std::shared_ptr<Type> Types::getVoidNode()
  {
    return std::make_shared<Type>(TypeVoid::instance, 0);
  }
  
  std::shared_ptr<Type> Types::getBooleanNode()
  {
    return std::make_shared<Type>(TypeBoolean::instance, 0);
  }
  
  std::shared_ptr<Type> Types::getIntNode()
  {
    return std::make_shared<Type>(TypeInt::instance, 0);
  }
  
  std::shared_ptr<Type> Types::getFakeNode()
  {
    return std::make_shared<Type>(FakeType::instance, 0);
  }
  
  std::shared_ptr<Type> Types::getNullNode()
  {
    return std::make_shared<Type>(NullType::instance, 0);
  }
  
  static std::shared_ptr<Type> getIntNode();
  static std::shared_ptr<Type> getFakeType();
  static std::shared_ptr<Type> getBooleanNode();
  static std::shared_ptr<Type> getNullNode();
  
  /*
  void Types::init()
  {
    assert(TypeVoid::instance != nullptr);
    assert(TypeInt::instance != nullptr);
    assert(FakeType::instance != nullptr);
    assert(TypeBoolean::instance != nullptr);
    assert(NullType::instance != nullptr);
    
    // singletons for simple Types (arrayDepth = 0)
    voidNode    = std::make_shared<Type>(TypeVoid::instance, 0);
    intNode     = std::make_shared<Type>(TypeInt::instance, 0);
    fakeType    = std::make_shared<Type>(FakeType::instance, 0);
    booleanNode = std::make_shared<Type>(TypeBoolean::instance, 0);
    nullNode    = std::make_shared<Type>(NullType::instance, 0);
  }
  */
}
