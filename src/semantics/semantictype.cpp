#include "semantictype.h"

using namespace cmpl;

bool SemanticType::isValidSemanticType() {
  if (TypeVoid *v = dynamic_cast<TypeVoid*>(astType->type.get())) {
    return true;
  } else {
    return false;
  }
}
