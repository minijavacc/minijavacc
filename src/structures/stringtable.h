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
      TokenType tokenType;               // if entry is a keyword
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
      static const StringIdentifier strString;
      static const StringIdentifier strout;
      static const StringIdentifier strin;
      static const StringIdentifier strprintln;
      static const StringIdentifier strwrite;
      static const StringIdentifier strflush;
      static const StringIdentifier strread;
      
    private:
      StringTable() {
/*
        strflush   = insertString("flush");
        strin      = insertString("in");
        strmain    = insertString("main");
        strout     = insertString("out");
        strprintln = insertString("println");
        strString  = insertString("String");
        strread    = insertString("read");
        strwrite   = insertString("write");
      */};
      static std::map<std::string, StringTableContainer> map;
;
  };
  
  class StringTableNotFound : public std::runtime_error
  {
    public:
      StringTableNotFound(const char* err) : std::runtime_error(err) { };
  };

}
