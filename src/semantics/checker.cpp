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
//#include "staticresolver.h"
#include "node.h"
//#include "parser.h"

#include <iostream>

using namespace cmpl;


void Checker::run() {
  
  std::shared_ptr<Node> n = parser.getAST();

  // check for missing return paths
  std::shared_ptr<ReturnChecker> rc(new ReturnChecker());
  try
  {
    n->accept(rc);
    std::cout << "return check: valid!\n";
  }
  catch (SemanticError &e)
  {
     std::cout << "return check: " << e.what() << "\n";
  }
  
  // collect static declarations and check for duplicated methods and classes
  std::shared_ptr<StaticDeclarationsCollector> coll(new StaticDeclarationsCollector());
  try
  {
    n->accept(coll);
    std::cout << "static ceclarations check: valid!\n";
  }
  catch (SemanticError &e)
  {
     std::cout << "static declarations check: " << e.what() << "\n";
  }
  
  //std::shared_ptr<StaticResolver> resolver(new StaticResolver());
  //n->accept(resolver);
 
}
