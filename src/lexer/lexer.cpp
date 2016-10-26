#include "lexer.h"
#include "token.h"
#include "compiler.h"

#include <iostream>
#include <fstream>
#include <memory>

using namespace cmpl;

/*
 * following operators/seperator have to be implemented in state machine:
  != (done)
  !  (done)
  (  (done)
  )  (done)
  *=
  *
  ++
  +=
  +
  ,
  -=
  --
  -
  .
  /=,
  /
  :
  ;
  <<=
  <<
  <=
  <
  ==
  =
  >=
  >>=
  >>>=
  >>>
  >>
  >
  ?
  %=
  %
  &=
  &&
  &
  [
  ],
  ^=
  ^
  {
  }
  ~
  |=
  ||
  |
 */

/*
 * - currentChar: contains the character, that is currently processed
 * - currentTokenString: contains the string with all valid characters that have been collected for this token so far
 * - the current character is checked whether it still belongs to the current token 
 *   if not, the last token is finished and inserted into the token array
 */ 
void Lexer::run(std::ifstream &inputFile)
{
  char currentChar;
  std::string currentTokenString = "";
  
  if (!inputFile)
    throw std::invalid_argument("no valid input file given");
  
  // process each character of the stream
  while(inputFile.get(currentChar))
  {
    // states of the state machine have labels starting with 's_'
    
    // TODO: handle EOF
    
    s_0:
    switch (currentChar)
    {
      // integer
      case '1'...'9':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_int;
      
      // integer
      case '0':
        insertToken(std::make_unique<IntegerLiteralToken>(0));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_0;
      
      case '!':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_excl;
      
      case '(':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LPAREN));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_0;
        
      case ')':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_RPAREN));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_0;
      
      
      
      // ...
      
      
      
      // white spaces
      case ' ':
      case '\r':
      case '\t':
      case '\n':
        // skip this character
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_0;
      
      // string or keyword
      case 'a'...'z':
      case 'A'...'Z':
      case '_':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_str;
      
      default:
        throw SyntaxError();
    }
    
    s_int:
    switch (currentChar)
    {
      // integer
      case '0'...'9':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_int;
      
      default:
        uint32_t value = std::stoi(currentTokenString);
        insertToken(std::make_unique<IntegerLiteralToken>(value));
        
        // begin next token
        currentTokenString = "";
        goto s_0;
    }
    
    s_str:
    switch (currentChar)
    {
      case 'a'...'z':
      case 'A'...'Z':
      case '_':
      case '0'...'9':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_str;
      
      default:
        // lookup in string table and add the returning token
        // string table automatically checks if token is keyword
        auto token = stringTable.insertString(currentTokenString);
        insertToken(std::move(token));
        
        // begin next token
        currentTokenString = "";
        goto s_0;
    }
    
    s_excl:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EXCLM_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar))
          goto s_eof;
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EXCLM));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    
    
    // ...
    
    
    
    s_eof:
    return;
  }
}

void Lexer::insertToken(std::unique_ptr<Token> token)
{
  // later this maybe can be used for callbacks etc.
  tokenArray.push_back(std::move(token));
  
  std::cout << "sizeof(tokenArray): " << tokenArray.size() << "\n";
}

