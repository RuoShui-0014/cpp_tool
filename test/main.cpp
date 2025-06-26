#include <format>
#include <iostream>

#include "base/logger.h"
#include "base/safe_ptr.h"
#include "base/thread_pool.h"

long fibonacci(unsigned n) {
  if (n < 2) {
    return n;
  }
  return fibonacci(n - 1) + fibonacci(n - 2);
}

std::atomic_int data{0};

int main(int argc, char* argv[]) {
  base::Logger::Initialize("out/log.txt", base::Logger::Level::kDebug);
  LOG_INFO("Proccess entry ...");

  base::ThreadPool pool;
  pool.Start({20, 8});

  std::mutex mutex;
  for (int i = 0; i < 40; i++) {
    pool.AddTask(
        [&](int id) {
          LOG_DEBUG(std::format("{:0>3}->{}", id, "Thread task..."));

          auto start = std::chrono::high_resolution_clock::now();
          auto value = fibonacci(40);
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

  // std::this_thread::sleep_for(std::chrono::seconds(5));

  return 0;
}
