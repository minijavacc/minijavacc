#pragma once

// forward declarations
namespace cmpl
{
  class StringTable;
}

#include "token.h"

#include <map>
#include <memory>

namespace cmpl
{
  
  typedef struct
  { 
    bool isKeyword;
    
    // entry is either string or keyword, so the IDs can share their memory
    union
    {
      StringIdentifier stringIdentifier; // if entry is a string
      TokenType tokenType;                 // if entry is a keyword
    };
  }
  StringTableContainer;
  
  class StringTable
  {
    public:
      static std::unique_ptr<Token> insertString(std::string string, unsigned int line, unsigned int column);
      static void insertKeyword(std::string string, TokenType type);
      static std::string lookupIdentifier(StringIdentifier id);
      
      static const StringIdentifier invalidIdentifier = 0;
      
    private:
      StringTable() { };
      static std::map<std::string, StringTableContainer> map;
;
  };
  
  class StringTableNotFound : public std::runtime_error
  {
    public:
      StringTableNotFound(const char* err) : std::runtime_error(err) { };
  };

}
