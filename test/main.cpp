#include <format>
#include <iostream>

#include "../base/thread_pool.h"

long fibonacci(unsigned n) {
  if (n < 2) {
    return n;
  }
  return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char* argv[]) {
  base::ThreadPool pool;
  pool.Start({20, 10});

  std::mutex mutex;
  for (int i = 0; i < 20; i++) {
    pool.AddTask(
        [&](int id) {
          auto start = std::chrono::high_resolution_clock::now();
          auto value = fibonacci(45);
          auto end = std::chrono::high_resolution_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::seconds>(end - start);
          {
            std::lock_guard lock(mutex);
            std::cout << std::format("{:0>3} fibonacci = {}, time {}s", id,
                                     value, duration.count())
                      << std::endl;
          }
        },
        i);
  }

  return 0;
}
