#pragma once

#include <stdint.h>

namespace cmpl
{
  
  // operators,seperators T_O_*
  // keywords (only the ones used by MiniJava) T_K_*
  // all information from "MiniJava Sprachbericht"
  typedef enum
  {
    T_O_EXCLM_EQUAL, 
    T_O_EXCLM, 
    T_O_LPAREN, 
    T_O_RPAREN, 
    T_O_STAR_EQUAL, 
    T_O_STAR, 
    T_O_PLUS_PLUS, 
    T_O_PLUS_EQUAL, 
    T_O_PLUS, 
    T_O_KOMMA, 
    T_O_MINUS_EQUAL, 
    T_O_MINUS_MINUS, 
    T_O_MINUS, 
    T_O_DOT, 
    T_O_SLASH_EQUAL, 
    T_O_SLASH, 
    T_O_COLON, 
    T_O_SEMICOLON, 
    T_O_LESS_LESS_EQUAL, 
    T_O_LESS_LESS, 
    T_O_LESS_EQUAL, 
    T_O_LESS, 
    T_O_EQUAL_EQUAL, 
    T_O_EQUAL, 
    T_O_MORE_EQUAL, 
    T_O_MORE_MORE_EQUAL, 
    T_O_MORE_MORE_MORE_EQUAL, 
    T_O_MORE_MORE_MORE, 
    T_O_MORE_MORE, 
    T_O_MORE, 
    T_O_QUESTM, 
    T_O_PERCENT_EQUAL, 
    T_O_PERCENT, 
    T_O_AND_EQUAL, 
    T_O_AND_AND, 
    T_O_AND, 
    T_O_LBRACK, 
    T_O_RBRACK, 
    T_O_CARET_EQUAL, 
    T_O_CARET, 
    T_O_LBRACE, 
    T_O_RBRACE, 
    T_O_TILDE, 
    T_O_PIPE_EQUAL, 
    T_O_PIPE_PIPE, 
    T_O_PIPE, 
    
    T_K_BOOLEAN,  
    T_K_CLASS,  
    T_K_ELSE,  
    T_K_FALSE,  
    T_K_IF,  
    T_K_INT,  
    T_K_NEW,  
    T_K_NULL,  
    T_K_PUBLIC,  
    T_K_RETURN,  
    T_K_STATIC,  // no real keyword, only to detect main function
    T_K_THIS,  
    T_K_TRUE,  
    T_K_VOID,  
    T_K_WHILE, 
  }
  TokenType;
  
  typedef int IdentifierTokenId;

  class Token
  {
    // TODO: add information about token position in source code
  };
  
  class OperatorSeperatorKeywordToken : public Token
  {
    public:
      TokenType type;
  };
  
  class IdentifierToken : public Token
  {
    public:
      IdentifierTokenId id;
  };
  
  
  class IntegerLiteralToken : public Token
  {
    public:
      uint32_t value;
  };

}
