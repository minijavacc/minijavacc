#include "creator.h"
#include "typecreator.h"
#include "irbuilder.h"
#include "typecreator.h"

using namespace cmpl;

Creator::Creator(std::shared_ptr<Node> ast) : ast(ast)
{
  ir_init(); // initialize libfirm
}

Creator::~Creator()
{
  ir_finish(); // clean up
}

void Creator::run()
{
  // check for missing return paths
  std::shared_ptr<TypeCreator> t = std::make_shared<TypeCreator>();
  ast->accept(t);
  
  std::shared_ptr<IRBuilder> c = std::make_shared<IRBuilder>();
  ast->accept(c);
}

void Creator::dump()
{
  dump_all_ir_graphs("");
}
