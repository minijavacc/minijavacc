//
//  checker.hpp
//  mjcc
//
//  Created by Markus Schlegel on 17/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "parser.h"


namespace cmpl
{
    
  class Checker
  {
  public:
    Checker(Parser &parser) : parser(parser) { };
    void run();
  private:
    Parser &parser;
  };
  
  class SemanticError : public std::runtime_error
  {
    public:
      // TODO: think about a way to create useful error messages for semantic errors
      // e.g. should every Node have a reference to the token it was built from?
      SemanticError(const char* err) : std::runtime_error(err) { }
      SemanticError(const char* err, unsigned int line, unsigned int column)
        : std::runtime_error(err), line(line), column(column) { }
        
      unsigned int line;
      unsigned int column;
  };
  
}
