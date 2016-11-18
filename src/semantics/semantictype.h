#pragma once

#include "node.h"



namespace cmpl
{
  
  class SemanticType
  {
  public:
    bool isLValue = false;
    std::shared_ptr<Type> astType;
    bool isValidSemanticType(); // Semantic types type expressions, expressions cannot be void
    SemanticType(std::weak_ptr<Type> t) : astType(t) { };
  };

}
