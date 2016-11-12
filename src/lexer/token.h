#pragma once

// include file "stringtable.h" has been moved down to avoid cyclic dependencies

#include <stdint.h>
#include <string>

// forward declarations
namespace cmpl
{
  // operators,seperators T_O_*
  // keywords T_K_*
  // all information from "MiniJava Sprachbericht"
  typedef enum
  {
    T_O_EQUAL,
    T_O_PIPE_PIPE,
    T_O_AND_AND,
    T_O_EXCLM_EQUAL,
    T_O_EQUAL_EQUAL,
    T_O_LESS,
    T_O_MORE,
    T_O_LESS_EQUAL,
    T_O_MORE_EQUAL,
    T_O_PLUS,
    T_O_MINUS,
    T_O_SLASH,
    T_O_STAR,
    T_O_PERCENT,
    T_O_EXCLM,
    T_O_PLUS_EQUAL,
    T_O_MINUS_EQUAL,
    T_O_STAR_EQUAL,
    T_O_SLASH_EQUAL,
    T_O_LESS_LESS_EQUAL,
    T_O_MORE_MORE_EQUAL,
    T_O_MORE_MORE_MORE_EQUAL,
    T_O_PERCENT_EQUAL,
    T_O_AND_EQUAL,
    T_O_CARET_EQUAL,
    T_O_PIPE_EQUAL,
    T_O_PIPE,
    T_O_CARET,
    T_O_AND,
    T_O_LESS_LESS,
    T_O_MORE_MORE_MORE,
    T_O_MORE_MORE,
    T_O_TILDE,
    T_O_PLUS_PLUS,
    T_O_MINUS_MINUS,
    T_O_QUESTM,
    T_O_LPAREN,
    T_O_RPAREN,
    T_O_COMMA,
    T_O_DOT,
    T_O_COLON,
    T_O_SEMICOLON,
    T_O_LBRACK,
    T_O_RBRACK,
    T_O_LBRACE,
    T_O_RBRACE,
  
    T_K_ABSTRACT,
    T_K_ASSERT,
    T_K_BOOLEAN,
    T_K_BREAK,
    T_K_BYTE,
    T_K_CASE,
    T_K_CATCH,
    T_K_CHAR,
    T_K_CLASS,
    T_K_CONST,
    T_K_CONTINUE,
    T_K_DEFAULT,
    T_K_DOUBLE,
    T_K_DO,
    T_K_ELSE,
    T_K_ENUM,
    T_K_EXTENDS,
    T_K_FALSE,
    T_K_FINALLY,
    T_K_FINAL,
    T_K_FLOAT,
    T_K_FOR,
    T_K_GOTO,
    T_K_IF,
    T_K_IMPLEMENTS,
    T_K_IMPORT,
    T_K_INSTANCEOF,
    T_K_INTERFACE,
    T_K_INT,
    T_K_LONG,
    T_K_NATIVE,
    T_K_NEW,
    T_K_NULL,
    T_K_PACKAGE,
    T_K_PRIVATE,
    T_K_PROTECTED,
    T_K_PUBLIC,
    T_K_RETURN,
    T_K_SHORT,
    T_K_STATIC,
    T_K_STRICTFP,
    T_K_SUPER,
    T_K_SWITCH,
    T_K_SYNCHRONIZED,
    T_K_THIS,
    T_K_THROWS,
    T_K_THROW,
    T_K_TRANSIENT,
    T_K_TRUE,
    T_K_TRY,
    T_K_VOID,
    T_K_VOLATILE,
    T_K_WHILE
  }
  TokenType;
  
  typedef enum
  {
    left,
    right,
    none
  }
  OperatorAssociativity;
  
  typedef enum
  {
    unaryOperator,
    binaryOperator,
    unusedOperator,
    seperator,
    keyword
  }
  TokenCategory;
  
  struct OperatorSeperatorKeywordContainer
  {
      const TokenCategory category;
      const std::string stringRepresentation;
      const unsigned int precedence;
      const OperatorAssociativity associativity;
  };
  
  typedef int IdentifierTokenId;
  
  class Token;
}

#include "stringtable.h"

namespace cmpl
{
  class Token
  {
    public:
      Token(unsigned int line, unsigned int column) 
        : line(line), column(column) {};
      virtual std::string getStringValue() = 0;
      const static OperatorSeperatorKeywordContainer tokenAttribues[99];
      
      unsigned int line;
      unsigned int column;
  };
  
  class OperatorSeperatorKeywordToken : public Token
  {
    public:
      OperatorSeperatorKeywordToken(TokenType type, unsigned int line, unsigned int column)
        : Token(line, column), type(type) {};
      TokenType type;
      std::string getStringValue();
  };
  
  class IdentifierToken : public Token
  {
    public:
      IdentifierToken(IdentifierTokenId id, StringTable &stringTable, unsigned int line, unsigned int column)
        : Token(line, column), id(id), stringTable(stringTable) {};
      StringTable &stringTable;
      IdentifierTokenId id;
      std::string getStringValue();
  };
  
  class IntegerLiteralToken : public Token
  {
    public:
      IntegerLiteralToken(std::string value, unsigned int line, unsigned int column)
        : Token(line, column), value(value) {};
      std::string value;
      std::string getStringValue();
  };

}
