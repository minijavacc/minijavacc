//
//  optimizer.h
//  mjcc
//
//  Created by Markus Schlegel on 19/01/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#pragma once

#include "creator.h"

namespace cmpl
{
  
  class Optimizer
  {
  public:
    Optimizer(Creator &creator) : creator(creator) {}
    void run();
    
  private:
    Creator &creator;
  };
}

