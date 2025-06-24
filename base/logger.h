#pragma once
#include <atomic>
#include <format>
#include <fstream>
#include <mutex>
#include <queue>
#include <source_location>
#include <string>
#include <thread>

namespace base {

class Logger {
 public:
  enum class Level { kDebug, kInfo, kWarning, kError, kCritical };

  static Logger& Get();
  static void Initialize(std::string file, Level level);

  static void Log(
      Level level,
      std::string message,
      const std::source_location& location = std::source_location::current());

  ~Logger();

 private:
  struct Info {
    Level level;
    std::string message;
    std::source_location location;
  };
  Logger();

  Level level_;
  std::atomic_bool initialized_;
  std::atomic_bool running_;

  std::ofstream file_;
  std::thread thread_;
  std::mutex mutex_;
  std::queue<Info> queue_;
  std::condition_variable cv_;
};

}  // namespace base
