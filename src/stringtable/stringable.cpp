#include "stringtable.h"
#include "token.h"

#include <unordered_map>

using namespace cmpl;

IdentifierTokenId StringTable::insertString(std::string string)
{
  // check if element exists already
  if (map.count(string) > 0)
  {
    // check if string is keyword
    if (map[string].isKeyword) // if so, throw exception
      throw StringIsKeywordException();
    else // if not, return IdentifierTokenId
      return map[string].id;
  }
  else
  {
    // insert string into map and give it a new IdentifierTokenId
    IdentifierTokenId newIdentifierTokenId = nextIdentifierTokenId++; // post-increment
    StringTableContainer container = {newIdentifierTokenId, false};
    
    map.insert(std::make_pair(string, container));
    
    return newIdentifierTokenId;
  }
}

void StringTable::insertKeyword(std::string string)
{
  StringTableContainer container = {0, true};
  
  map.insert(std::make_pair(string, container));
}
