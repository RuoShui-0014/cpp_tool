#include <format>
#include <iostream>

#include "../base/thread_pool.h"

template <typename Func, typename... Args>
auto measure_time(Func&& func, Args&&... args) {
  // 获取开始时间点
  auto start = std::chrono::high_resolution_clock::now();

  // 执行传入的函数
  auto result = std::forward<Func>(func)(std::forward<Args>(args)...);

  // 获取结束时间点
  auto end = std::chrono::high_resolution_clock::now();

  // 计算耗时
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "All time: " << duration.count() << " us" << std::endl;

  return result;
}

long fibonacci(unsigned n) {
  if (n < 2) {
    return n;
  }
  return fibonacci(n - 1) + fibonacci(n - 2);
}



int main(int argc, char* argv[]) {
  base::ThreadPool pool;
  pool.Start({20, 20});

  std::string msg = "ssssssssss";
  for (int i = 0; i < 20; i++) {
    pool.AddTask([msg](int id) {
      auto fib_result = measure_time(fibonacci, 30);
      std::cout << std::format("{} fibonacci(30) = ", msg.size()) << fib_result << std::endl;
    }, i);
  }

  std::this_thread::sleep_for(std::chrono::seconds(12));

  return 0;
}
