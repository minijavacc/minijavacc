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
  *= (done)
  *  (done)
  ++ (done)
  += (done)
  +  (done)
  ,  (done)
  -= (done)
  -- (done)
  -  (done)
  .  (done)
  /= (done)
  /  (done)
  :  (done)
  ;  (done)
  <<=(done)
  << (done)
  <= (done)
  <  (done)
  == (done)
  =  (done)
  >= (done)
  >>=(done)
  >>>=(done)
  >>>(done)
  >> (done)
  >  (done)
  ?  (done)
  %= (done)
  %  (done)
  &= (done)
  && (done)
  &  (done)
  [  (done)
  ]  (done)
  ^= (done)
  ^  (done)
  {  (done)
  }  (done)
  ~  (done)
  |= (done)
  || (done)
  |  (done)
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
  
  if (!inputFile) {
    throw std::invalid_argument("no valid input file given");
  }
  
  // process each character of the stream
  while(inputFile.get(currentChar))
  {
    // states of the state machine have labels starting with 's_'
    
    s_0:
    switch (currentChar)
    {
      // integer
      case '1'...'9':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          uint32_t value = std::stoi(currentTokenString);
          insertToken(std::make_unique<IntegerLiteralToken>(value));
          goto s_eof;
        }
        goto s_int;
      
      // integer
      case '0':
        insertToken(std::make_unique<IntegerLiteralToken>(0));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '!':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EXCLM));
          goto s_eof;
        }
        goto s_excl;
      
      case '(':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LPAREN));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
        
      case ')':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_RPAREN));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '/':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_SLASH));
          goto s_eof;
        }
        goto s_slash;
      
      case '*':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_STAR));
          goto s_eof;
        }
        goto s_star; 
      
      case '+':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PLUS));
          goto s_eof;
        }
        goto s_plus; 
      
      case ',':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_KOMMA));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '-':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MINUS));
          goto s_eof;
        }
        goto s_minus; 
      
      case '.':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_DOT));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case ':':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_COLON));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case ';':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_SEMICOLON));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '<':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS));
          goto s_eof;
        }
        goto s_less_1;
        
      case '=':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EQUAL));
          goto s_eof;
        }
        goto s_equal; 
      
      case '>':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_more_1;
      
      case '?':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_QUESTM));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '%':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PERCENT));
          goto s_eof;
        }
        goto s_percent;
      
      case '&':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_AND));
          goto s_eof;
        }
        goto s_and;
      
      case '[':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LBRACK));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case ']':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_RBRACK));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '^':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_CARET));
          goto s_eof;
        }
        goto s_caret;
      
      case '{':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LBRACE));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '}':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_RBRACE));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '~':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_TILDE));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
        
      case '|':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PIPE));
          goto s_eof;
        }
        goto s_pipe;
      
      // white spaces
      case ' ':
      case '\r':
      case '\t':
      case '\n':
        // skip this character
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      // string or keyword
      case 'a'...'z':
      case 'A'...'Z':
      case '_':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          auto token = stringTable.insertString(currentTokenString);
          insertToken(std::move(token));
          goto s_eof;
        }
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
        if (!inputFile.get(currentChar)) {
          uint32_t value = std::stoi(currentTokenString);
          insertToken(std::make_unique<IntegerLiteralToken>(value));
          goto s_eof;
        }
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
        if (!inputFile.get(currentChar)) {
          auto token = stringTable.insertString(currentTokenString);
          insertToken(std::move(token));
          goto s_eof;
        }
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
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EXCLM));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_star:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_STAR_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_STAR));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_plus:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PLUS_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '+':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PLUS_PLUS));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PLUS));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_minus:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MINUS_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '-':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MINUS_MINUS));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MINUS));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_less_1:
    switch (currentChar)
    {
      case '<':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS_LESS));
          goto s_eof;
        }
        goto s_less_2;
      
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_less_2:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS_LESS_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS_LESS));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_equal:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EQUAL_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_more_1:
    switch (currentChar)
    {
      case '>':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE));
          goto s_eof;
        }
        goto s_more_2;
      
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_more_2:
    switch (currentChar)
    {
      case '>':
        // continue in state machine
        currentTokenString += currentChar;        
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE_MORE));
          goto s_eof;
        }
        goto s_more_3;
      
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_more_3:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE_MORE_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE_MORE));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_slash:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_SLASH_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      // found begin of comment
      case '*':
        // continue in state machine
        if (!inputFile.get(currentChar)) {
          throw SyntaxError();
        }
        goto s_comment_1;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_SLASH));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_comment_1:
    switch (currentChar)
    {
      case '*':
        // continue in state machine
        if (!inputFile.get(currentChar)) {
          throw SyntaxError();
        }
        goto s_comment_2;
      
      default:
        // continue in state machine
        if (!inputFile.get(currentChar)) {
          throw SyntaxError();
        }
        goto s_comment_1;
    }
    
    s_comment_2:
    switch (currentChar)
    {
      case '/':
        // for a comment to token will be inserted
      
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '*':
        // continue in state machin
        if (!inputFile.get(currentChar)) {
          throw SyntaxError();
        }
        goto s_comment_2;
      
      // comment did not end -> continue as comment
      default:
        // continue in state machine
        if (!inputFile.get(currentChar)) {
          throw SyntaxError();
        }
        goto s_comment_1;
    }
    
    s_percent:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PERCENT_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PERCENT));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_and:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_AND_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '&':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_AND_AND));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_AND));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_caret:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_CARET_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_CARET));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_pipe:
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PIPE_EQUAL));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '|':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PIPE_PIPE));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PIPE));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_eof:
    return;
  }
}
Lexer::Lexer() : stringTable(StringTable())
{
  // Set default string table
  // this.stringTable = StringTable();
  
  for (int i = T_K_ABSTRACT; i <= T_K_WHILE; i++)
  {
    stringTable.insertKeyword(Token::stringRepresentations[i], (TokenType)i);
  }      
}

inline void Lexer::insertToken(std::unique_ptr<Token> token)
{
  // later this maybe can be used for callbacks etc.
  tokenArray.push(std::move(token));
}

bool Lexer::getNextToken(std::unique_ptr<Token> &t)
{
  if (tokenArray.empty())
  {
    return false;
  }
  
  std::unique_ptr<Token> token = std::move(tokenArray.front());
  tokenArray.pop();
  t = std::move(token);
  
  return true;
}

