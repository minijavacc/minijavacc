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
#include "voidtypechecker.h"
#include "typechecker.h"
#include "mainmethodchecker.h"
#include "ast.h"

#include <iostream>

using namespace cmpl;


void Checker::run() {
  
  std::shared_ptr<Node> n = parser.getAST();

  // check for missing return paths
  std::shared_ptr<ReturnChecker> rc(new ReturnChecker());
  n->accept(rc);
  
  // collect static declarations and check for duplicated methods and classes
  std::shared_ptr<StaticDeclarationsCollector> coll(new StaticDeclarationsCollector());
  n->accept(coll);
  
  // resolve all static references (CRefs)
  std::shared_ptr<StaticResolver> res(new StaticResolver());
  n->accept(res);
  
  // check main method
  std::shared_ptr<MainMethodChecker> mc(new MainMethodChecker());
  n->accept(mc);

  // void type check
  std::shared_ptr<VoidTypeChecker> voidcheck(new VoidTypeChecker());
  n->accept(voidcheck);
  
  // type check
  std::shared_ptr<TypeChecker> typcheck(new TypeChecker());
  n->accept(typcheck);

  std::cout << "all semantic checks passed\n";
}
