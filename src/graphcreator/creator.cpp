#include "creator.h"
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
  std::shared_ptr<TypeCreator> tc = std::make_shared<TypeCreator>();
  ast->accept(tc);
}

void Creator::dump()
{
  dump_all_ir_graphs("");
}
