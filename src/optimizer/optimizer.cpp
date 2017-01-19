//
//  optimizer.cpp
//  mjcc
//
//  Created by Markus Schlegel on 19/01/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#include "optimizer.h"
#include "constantpropagation.h"
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
    
    irg_finalize_cons(irg);
  }
}