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
      IdentifierTokenId identifierTokenId; // if entry is a string
      TokenType tokenType;                 // if entry is a keyword
    };
  }
  StringTableContainer;
  
  class StringTable
  {
    public:
      std::unique_ptr<Token> insertString(std::string string, unsigned int line, unsigned int column);
      void insertKeyword(std::string string, TokenType type);
      std::string lookupIdentifier(IdentifierTokenId id);
      
    private:
      std::map<std::string, StringTableContainer> map;
      IdentifierTokenId nextIdentifierTokenId = 0; // ugly, but didn't find any better solution (maybe hash of the string?)
  };
  
  class StringTableNotFound : public std::exception { };

}
