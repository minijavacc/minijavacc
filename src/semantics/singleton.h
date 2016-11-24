#pragma once

namespace cmpl
{
  // singletons for Types
  const std::shared_ptr<Type> voidNode = std::make_shared<Type>(std::make_shared<TypeVoid>(), 0);
  const std::shared_ptr<Type> intNode = std::make_shared<Type>(std::make_shared<TypeInt>(), 0);
  const std::shared_ptr<Type> booleanNode = std::make_shared<Type>(std::make_shared<TypeBoolean>(), 0);
  template <StringIdentifier ID, int arrayDepth>
  const std::shared_ptr<Type> userNode = std::make_shared<Type>(std::make_shared<UserType>(ID), arrayDepth);
  
  const std::shared_ptr<BasicType> voidBasicNode = std::make_shared<TypeVoid>();
}
