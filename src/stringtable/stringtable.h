#pragma once

#include "token.h"

#include <unordered_map>

namespace cmpl
{
  
  typedef struct
  { 
    IdentifierTokenId id;
    bool isKeyword;
  }
  StringTableContainer;
  
  class StringTable
  {
    public:
      IdentifierTokenId insertString(std::string string); // throws Exception if string is keyword
      void insertKeyword(std::string string);
      
    private:
      std::unordered_map<std::string, StringTableContainer> map;
      IdentifierTokenId nextIdentifierTokenId = 0;
  };
  
  class StringIsKeywordException : public std::exception {};

}
