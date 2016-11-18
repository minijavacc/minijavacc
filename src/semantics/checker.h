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
  
}