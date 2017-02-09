//
//  optimizer.cpp
//  mjcc
//
//  Created by Markus Schlegel on 19/01/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#include "optimizer.h"
#include "constantpropagation.h"
#include "arithmeticoptimization.h"
#include <libfirm/firm.h>


using namespace cmpl;

void Optimizer::run()
{
  size_t irgsNum = get_irp_n_irgs();
  
  // iterate irgs
  for (int i = 0; i < irgsNum; i++)
  {
    ir_graph *irg = get_irp_irg(i);
    
    ConstantPropagation cp(irg);
    cp.run();
	
	ArithmeticOptimization ao(irg);
	ao.run();
    
    irg_finalize_cons(irg);
    
    // remove_* with libfirm
    remove_bads(irg);
    remove_unreachable_code(irg);
    remove_tuples(irg);
    
    lower_highlevel_graph(irg);
  }
  
}
