//
//  checker.cpp
//  mjcc
//
//  Created by Markus Schlegel on 17/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "checker.h"
#include "returnchecker.h"

#include <iostream>

using namespace cmpl;


void Checker::run() {
  
  std::unique_ptr<Node> n;
  parser.getAST(n);

  // walk the line
  
  ReturnChecker rc;
  
  n->accept(rc);
  
  if (rc.valid) {
    std::cout << "Return check: valid!\n";
  } else {
    std::cout << "Return check: invalid (missing return)!\n";
  }
}
