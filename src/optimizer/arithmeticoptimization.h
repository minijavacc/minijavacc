
#pragma once

#include <libfirm/firm.h>
#include "../structures/ast.h"

namespace cmpl {
  

	
  class ArithmeticOptimization : public std::enable_shared_from_this<ArithmeticOptimization> {
  public:
    ArithmeticOptimization(ir_graph* irg) : irg(irg) {};
    ir_graph* irg;
    void run();
  };
  
}
