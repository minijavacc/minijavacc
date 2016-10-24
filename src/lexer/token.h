#pragma once


namespace cmpl
{
  
  // operators and seperators in from "MiniJava Sprachbericht"
  typedef enum
  {
    T_EXCLM_EQUAL, 
    T_EXCLM, 
    T_LPAREN, 
    T_RPAREN, 
    T_STAR_EQUAL, 
    T_STAR, 
    T_PLUS_PLUS, 
    T_PLUS_EQUAL, 
    T_PLUS, 
    T_KOMMA, 
    T_MINUS_EQUAL, 
    T_MINUS_MINUS, 
    T_MINUS, 
    T_DOT, 
    T_SLASH_EQUAL, 
    T_SLASH, 
    T_COLON, 
    T_SEMICOLON, 
    T_LESS_LESS_EQUAL, 
    T_LESS_LESS, 
    T_LESS_EQUAL, 
    T_LESS, 
    T_EQUAL_EQUAL, 
    T_EQUAL, 
    T_MORE_EQUAL, 
    T_MORE_MORE_EQUAL, 
    T_MORE_MORE_MORE_EQUAL, 
    T_MORE_MORE_MORE, 
    T_MORE_MORE, 
    T_MORE, 
    T_QUESTM, 
    T_PERCENT_EQUAL, 
    T_PERCENT, 
    T_AND_EQUAL, 
    T_AND_AND, 
    T_AND, 
    T_LBRACK, 
    T_RBRACK, 
    T_CARET_EQUAL, 
    T_CARET, 
    T_LBRACE, 
    T_RBRACE, 
    T_TILDE, 
    T_PIPE_EQUAL, 
    T_PIPE_PIPE, 
    T_PIPE, 
  }
  TokenType;

  class Token
  {
  };
  
  class OperatorSeperatorToken : public Token
  {
    public:
      
  };
  
  class IdentifierToken : public Token
  {
    
  };
  
  class KeywordToken : public Token
  {
    
  };
  
  class IntegerLiteralToken : public Token
  {
    
  };

}
