//
//  checker.cpp
//  mjcc
//
//  Created by Markus Schlegel on 17/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "checker.h"
#include "returnchecker.h"
#include "staticdeclarationscollector.h"
#include "staticresolver.h"

#include <iostream>

using namespace cmpl;


void Checker::run() {
  
  std::shared_ptr<Node> n = parser.getAST();

  // walk the line
  
//  std::shared_ptr<ReturnChecker> rc(new ReturnChecker());
//  
//  n->accept(rc);
//  
//  if (rc->valid) {
//    std::cout << "Return check: valid!\n";
//  } else {
//    std::cout << "Return check: invalid (missing return)!\n";
//  }
  
  std::shared_ptr<StaticDeclarationsCollector> coll(new StaticDeclarationsCollector());
  std::shared_ptr<StaticResolver> resolver(new StaticResolver());
  
  n->accept(coll);
  n->accept(resolver);
}