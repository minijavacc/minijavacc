#include "stringtable.h"


#include <assert.h>
#include <iostream>

using namespace cmpl;

std::map<std::string, StringTableValue> StringTable::strings;
std::vector<std::string> StringTable::lookupTable;

void StringTable::insertKeyword(std::string string, TokenType tt) {
  strings.insert(std::make_pair(string, StringTableValue(tt)));
}

std::unique_ptr<Token> StringTable::insertString(std::string string, const unsigned int line, const unsigned int column)
{
  static StringIdentifier nextStringIdentifier = beginIDIndices;
  
  auto pair = strings.insert(std::make_pair(string, nextStringIdentifier));
  if(pair.second) { //new element inserted
    lookupTable.push_back(string);
    // create token for new string
    std::unique_ptr<Token> token = std::make_unique<IdentifierToken>(nextStringIdentifier++, line, column);
    return token;
  } else {
    // string already known
    StringTableValue value(strings[string]);
    if(value.stringIdentifier < beginIDIndices) {
      // keyword
      std::unique_ptr<Token> token = std::make_unique<OperatorSeperatorKeywordToken>(value.tokenType, line, column);
      return token;
    } else {
      // identifier
      std::unique_ptr<Token> token = std::make_unique<IdentifierToken>(value.stringIdentifier, line, column);
      return token;
    }
  }
}

std::string StringTable::lookupIdentifier(StringIdentifier id) {
  int index = id-beginIDIndices;
  if(0<=index && index<lookupTable.size()) {
    return lookupTable[index];
  } else {
    throw StringTableNotFound("lookupIdentifier: identifier not found");
  }
}
