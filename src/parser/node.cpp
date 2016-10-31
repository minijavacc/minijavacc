#include "node.h"

using namespace cmpl;

// example implementation to show getStringValue()
std::string Addition::getStringValue()
{
  return "(" + left->getStringValue() + " + " + right->getStringValue() + ")";
}
