#pragma once

#include "token.h"

#include <unordered_map>
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
      std::unique_ptr<Token> insertString(std::string string);
      void insertKeyword(std::string string, TokenType type);
      
    private:
      std::unordered_map<std::string, StringTableContainer> map;
      IdentifierTokenId nextIdentifierTokenId = 0;
  };

}
