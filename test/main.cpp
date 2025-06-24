#include <format>
#include <iostream>

#include "base/logger.h"
#include "base/thread_pool.h"

long fibonacci(unsigned n) {
  if (n < 2) {
    return n;
  }
  return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char* argv[]) {
  base::Logger::Initialize("out/log.txt", base::Logger::Level::kDebug);
  base::Logger::Log(base::Logger::Level::kDebug,
                    std::format("{}", "Proccess entry ..."));

  base::ThreadPool pool;
  pool.Start({20, 8});

  std::mutex mutex;
  for (int i = 0; i < 1000; i++) {
    pool.AddTask(
        [&](int id) {
          base::Logger::Log(base::Logger::Level::kDebug,
                            std::format("{:0>3}->{}", id, "Thread task ..."));

          auto start = std::chrono::high_resolution_clock::now();
          auto value = fibonacci(30);
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
