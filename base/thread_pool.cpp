#include "thread_pool.h"

#include <iostream>
#include <format>

namespace base {

Thread_pool::Thread_pool() = default;
Thread_pool::~Thread_pool() = default;

void Thread_pool::start() {
  std::cout << std::format("test") << std::endl;
}

}