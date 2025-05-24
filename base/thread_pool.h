#pragma once

#include "macro.h"

namespace base {

class BASE_EXPORT Thread_pool {
public:
  Thread_pool();
  ~Thread_pool();

  void start();
};

} // base
