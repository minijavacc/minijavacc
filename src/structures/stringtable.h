#pragma once

// forward declarations
namespace cmpl
{
  class StringTable;
}

#include "token.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cmpl
{
  struct StringTableValue {
    // entry is either string or keyword, so the IDs can share their memory
    union
    {
      StringIdentifier stringIdentifier; // if entry is a string
      TokenType tokenType;               // if entry is a keyword
    };
    
    StringTableValue(StringIdentifier id) : stringIdentifier(id) { };
    StringTableValue(TokenType tt) : tokenType(tt) { };
    StringTableValue() { };
  };
  
  class StringTable
  {
    public:
      static std::unique_ptr<Token> insertString(std::string string, const unsigned int line, const unsigned int column);
      static std::string lookupIdentifier(StringIdentifier id);
      static void insertKeyword(std::string string, TokenType tt);
      
      static std::map<std::string, StringTableValue> strings;
      static std::vector<std::string> lookupTable;
      
    private:
      StringTable() { };
      static const int beginIDIndices = T_K_WHILE + 1;
  };
  
  class StringTableNotFound : public std::runtime_error
  {
    public:
      StringTableNotFound(const char* err) : std::runtime_error(err) { };
  };
}
