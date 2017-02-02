//
//  checker.cpp
//  mjcc
//
//  Created by Markus Schlegel on 17/11/16.
//  Copyright © 2016 Markus Schlegel. All rights reserved.
//

#include "checker.h"
#include "mainmethodchecker.h"
#include "returnchecker.h"
#include "staticdeclarationscollector.h"
#include "staticresolver.h"
#include "typechecker.h"
#include "voidtypechecker.h"
#include "astmodifier.h"

#include "../structures/ast.h"
#include "../parser/prettyprinter.h"

#include <iostream>
#include <sstream>

using namespace cmpl;


void Checker::run() {
  
  std::shared_ptr<Node> n = parser.getAST();
  
  // check for missing return paths
  std::shared_ptr<ReturnChecker> rc(new ReturnChecker());
  n->accept(rc);
  
  // collect static declarations and check for duplicated methods and classes
  std::shared_ptr<StaticDeclarationsCollector> coll(new StaticDeclarationsCollector());
  n->accept(coll);
  
  // recongnise tree patterns and replace with other AST nodes
  // e.g. System.out.println, -CIntegerLiteral
  // CAREFUL: this call modifies the AST!
  std::shared_ptr<AstModifier> mod(new AstModifier());
  n->accept(mod);
  
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

std::string Checker::printNode(const std::shared_ptr<Node> &n) {
  std::stringstream stream;
  std::shared_ptr<Dispatcher> pp = std::make_shared<PrettyPrinter>(stream);
  n->accept(pp);
  return stream.str();
};

std::shared_ptr<Node> Checker::getAttributedAST()
{
  return parser.getAST();
}
