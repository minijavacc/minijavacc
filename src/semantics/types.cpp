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
  
}
