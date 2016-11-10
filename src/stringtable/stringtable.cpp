#include "stringtable.h"

#include <map>
#include <string>
#include <memory>

using namespace cmpl;

std::unique_ptr<Token> StringTable::insertString(std::string string, unsigned int line, unsigned int column)
{
  // check if element exists already
  if (map.count(string) > 0)
  {
    // check if string is keyword
    if (map[string].isKeyword)
    {
      // create token for keyword
      std::unique_ptr<Token> token = std::make_unique<OperatorSeperatorKeywordToken>(map[string].tokenType, line, column);
      return token;
    }
    else
    {
      // create token for existing string
      std::unique_ptr<Token> token = std::make_unique<IdentifierToken>(map[string].identifierTokenId, *this, line, column);
      return token;
    }
  }
  else
  {
    // insert string into map and get a new IdentifierTokenId
    IdentifierTokenId newIdentifierTokenId = nextIdentifierTokenId++; // post-increment!
    
    StringTableContainer container;
    container.isKeyword = false;
    container.identifierTokenId = newIdentifierTokenId;
    
    map.insert(std::make_pair(string, container));
    
    // create token for new string
    std::unique_ptr<Token> token = std::make_unique<IdentifierToken>(newIdentifierTokenId, *this, line, column);
    return token;
  }
}

void StringTable::insertKeyword(std::string string, TokenType type)
{
  StringTableContainer container;
    container.isKeyword = true;
    container.tokenType = type;
  
  map.insert(std::make_pair(string, container));
}

std::string StringTable::lookupIdentifier(IdentifierTokenId id)
{
  for (auto &pair : map)
  {
    if (pair.second.isKeyword == false && pair.second.identifierTokenId == id)
    {
      return pair.first;
    }
  }
  
  throw StringTableNotFound();
}
