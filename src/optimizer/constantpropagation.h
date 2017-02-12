//
//  ConstantPropagation.h
//  mjcc
//
//  Created by Markus Schlegel on 09/01/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#pragma once

#include <libfirm/firm.h>
#include "../structures/ast.h"

namespace cmpl {
  
  class ConstantPropagation : public std::enable_shared_from_this<ConstantPropagation> {
  public:
    ConstantPropagation(ir_graph* irg) : irg(irg) {};
    ir_graph* irg;
    void run();
  };
  
}
