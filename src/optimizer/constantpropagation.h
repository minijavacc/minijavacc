//
//  ConstantPropagation.h
//  mjcc
//
//  Created by Markus Schlegel on 09/01/17.
//  Copyright © 2017 Markus Schlegel. All rights reserved.
//

#pragma once

#include <libfirm/firm.h>

namespace cmpl {
  
  class ConstantPropagation : public Dispatcher, public std::enable_shared_from_this<IRInitializer> {
  }
}