#include "lexer.h"
#include "token.h"
#include "compiler.h"

#include <iostream>
#include <fstream>

using namespace cmpl;


void Lexer::run(std::ifstream inputFile)
{
  char currentChar;
  std::string currentTokenString = "";
  
  if (!inputFile)
    throw std::invalid_argument("no valid input file given");
  
  // process each character of the stream
  while(inputFile.get(&currentChar, 1))
  {
    // states of the state machine have labels starting with 's'
    
    // TODO: handle EOF
    
    s0:
    switch (currentChar)
    {
      // integer
      case '0'...'9':
        currentTokenString += currentChar;
        inputFile.get(&currentChar, 1);
        goto s1;
        
        // ...
      
      default:
        throw SyntaxError();
    }
    
    s1:
    switch (currentChar)
    {
      default:
        throw SyntaxError();
    }
    // ...
  }
}
