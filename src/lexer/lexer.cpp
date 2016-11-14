#include "lexer.h"

#include <iostream>
#include <fstream>
#include <memory>

using namespace cmpl;

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
  int line = 1;    // lines start with 1
  int column = 0;  // columns start with 0
  
  if (!inputFile) {
    throw std::invalid_argument("no valid input file given");
  }
  
  // process each character of the stream
  while(inputFile.get(currentChar))
  {
    // states of the state machine have labels starting with 's_'
    
    s_0:
    ++column;
    switch (currentChar)
    {
      // integer
      case '1'...'9':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<IntegerLiteralToken>(currentTokenString, line, column));
          goto s_eof;
        }
        goto s_int;
      
      // integer
      case '0':
        insertToken(std::make_unique<IntegerLiteralToken>("0", line, column));
        
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
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EXCLM, line, column));
          goto s_eof;
        }
        goto s_excl;
      
      case '(':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LPAREN, line, column));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
        
      case ')':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_RPAREN, line, column));
        
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
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_SLASH, line, column));
          goto s_eof;
        }
        goto s_slash;
      
      case '*':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_STAR, line, column));
          goto s_eof;
        }
        goto s_star; 
      
      case '+':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PLUS, line, column));
          goto s_eof;
        }
        goto s_plus; 
      
      case ',':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_COMMA, line, column));
        
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
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MINUS, line, column));
          goto s_eof;
        }
        goto s_minus; 
      
      case '.':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_DOT, line, column));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case ':':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_COLON, line, column));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case ';':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_SEMICOLON, line, column));
        
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
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS, line, column));
          goto s_eof;
        }
        goto s_less_1;
        
      case '=':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EQUAL, line, column));
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
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_QUESTM, line, column));
        
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
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PERCENT, line, column));
          goto s_eof;
        }
        goto s_percent;
      
      case '&':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_AND, line, column));
          goto s_eof;
        }
        goto s_and;
      
      case '[':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LBRACK, line, column));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case ']':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_RBRACK, line, column));
        
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
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_CARET, line, column));
          goto s_eof;
        }
        goto s_caret;
      
      case '{':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LBRACE, line, column));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '}':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_RBRACE, line, column));
        
        // begin next token
        currentTokenString = "";
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '~':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_TILDE, line, column));
        
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
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PIPE, line, column));
          goto s_eof;
        }
        goto s_pipe;
      
      // white spaces
      case '\n':
        line++;
        column = 0;
      case ' ':
      case '\r':
      case '\t':
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
          auto token = StringTable::insertString(currentTokenString, line, column);
          insertToken(std::move(token));
          goto s_eof;
        }
        goto s_str;
      
      default:
        throw SyntaxError();
    }
    
    s_int:
    ++column;
    switch (currentChar)
    {
      // integer
      case '0'...'9':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_int;
      
      default:
        insertToken(std::make_unique<IntegerLiteralToken>(currentTokenString, line, column));
        
        // begin next token
        currentTokenString = "";
        goto s_0;
    }
    
    s_str:
    ++column;
    switch (currentChar)
    {
      case 'a'...'z':
      case 'A'...'Z':
      case '_':
      case '0'...'9':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          auto token = StringTable::insertString(currentTokenString, line, column);
          insertToken(std::move(token));
          goto s_eof;
        }
        goto s_str;
      
      default:
        // lookup in string table and add the returning token
        // string table automatically checks if token is keyword
        auto token = StringTable::insertString(currentTokenString, line, column);
        insertToken(std::move(token));
        
        // begin next token
        currentTokenString = "";
        goto s_0;
    }
    
    s_excl:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EXCLM_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EXCLM, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_star:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_STAR_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_STAR, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_plus:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PLUS_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '+':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PLUS_PLUS, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PLUS, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_minus:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MINUS_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '-':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MINUS_MINUS, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MINUS, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_less_1:
    ++column;
    switch (currentChar)
    {
      case '<':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS_LESS, line, column));
          goto s_eof;
        }
        goto s_less_2;
      
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_less_2:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS_LESS_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_LESS_LESS, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_equal:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EQUAL_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_more_1:
    ++column;
    switch (currentChar)
    {
      case '>':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE, line, column));
          goto s_eof;
        }
        goto s_more_2;
      
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_more_2:
    ++column;
    switch (currentChar)
    {
      case '>':
        // continue in state machine
        currentTokenString += currentChar;
        if (!inputFile.get(currentChar)) {
          insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE_MORE, line, column));
          goto s_eof;
        }
        goto s_more_3;
      
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_more_3:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE_MORE_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_MORE_MORE_MORE, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_slash:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_SLASH_EQUAL, line, column));
        
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
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_SLASH, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_comment_1:
    ++column;
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
    ++column;
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
        // continue in state machine
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
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PERCENT_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PERCENT, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_and:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_AND_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '&':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_AND_AND, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_AND, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_caret:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_CARET_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_CARET, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_pipe:
    ++column;
    switch (currentChar)
    {
      case '=':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PIPE_EQUAL, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      case '|':
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PIPE_PIPE, line, column));
        
        // begin next token
        currentTokenString = "";
        
        if (!inputFile.get(currentChar)) {
          goto s_eof;
        }
        goto s_0;
      
      default:
        insertToken(std::make_unique<OperatorSeperatorKeywordToken>(T_O_PIPE, line, column));
        
        // begin next token
        currentTokenString = "";
        
        goto s_0;
    }
    
    s_eof:
    return;
  }
}

Lexer::Lexer()
{
  for (int i = T_K_ABSTRACT; i <= T_K_WHILE; i++)
  {
    StringTable::insertKeyword(Token::tokenAttribues[i].stringRepresentation, (TokenType)i);
  }
}


inline void Lexer::insertToken(std::unique_ptr<Token> token)
{
  // later this maybe can be used for callbacks etc.
  tokenArray.push_back(std::move(token));
}

bool Lexer::putBackToken(std::unique_ptr<Token> &t)
{
  tokenArray.push_front(std::move(t));
}

bool Lexer::getNextToken(std::unique_ptr<Token> &t)
{
  if (tokenArray.empty())
  {
    return false;
  }
  
  std::unique_ptr<Token> token = std::move(tokenArray.front());
  tokenArray.pop_front();
  t = std::move(token);
  
  return true;
}

bool Lexer:: hasNextToken() const
{
  return !tokenArray.empty();
}

