#include "stringtable.h"

#include <map>
#include <string>
#include <memory>

using namespace cmpl;

std::map<std::string, StringTableContainer> StringTable::map = std::map<std::string, StringTableContainer>();

std::unique_ptr<Token> StringTable::insertString(std::string string, unsigned int line, unsigned int column)
{
  // will keep counting upwards, as it's static (zero is invalid)
  static StringIdentifier nextStringIdentifier = 1;
  
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
      std::unique_ptr<Token> token = std::make_unique<IdentifierToken>(map[string].stringIdentifier, line, column);
      return token;
    }
  }
  else
  {
    // insert string into map and get a new StringIdentifier
    StringIdentifier newStringIdentifier = nextStringIdentifier++; // post-increment!
    
    StringTableContainer container;
    container.isKeyword = false;
    container.stringIdentifier = newStringIdentifier;
    
    map.insert(std::make_pair(string, container));
    
    // create token for new string
    std::unique_ptr<Token> token = std::make_unique<IdentifierToken>(newStringIdentifier, line, column);
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

std::string StringTable::lookupIdentifier(StringIdentifier id)
{
  for (auto &pair : map)
  {
    if (pair.second.isKeyword == false && pair.second.stringIdentifier == id)
    {
      return pair.first;
    }
  }
  
  if (id == invalidIdentifier)
  {
    // TODO: should not happen! quick fix to output System.out.printlf(param)
    return "";
  }
  
  throw StringTableNotFound("lookupIdentifier: identifier not found");
}
