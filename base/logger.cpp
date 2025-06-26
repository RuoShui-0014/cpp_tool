#include "logger.h"

#include <format>
#include <syncstream>

namespace base {

Logger& Logger::Get() {
  static Logger logger;
  return logger;
}

void Logger::Initialize(std::string file, Level level) {
  Logger& logger = Logger::Get();
  logger.file_.open(file, std::ios::out | std::ios::app);
  logger.level_ = level;

  logger.thread_ = std::thread([]() {
    static const char* leval_string[]{"DEBUG", "INFO", "WARN", "ERROR",
                                      "FATAL"};
    Logger& logger = Logger::Get();
    logger.running_.store(true);
    for (;;) {
      {
        std::unique_lock lock(logger.mutex_);
        logger.cv_.wait_until(
            lock,
            std::chrono::system_clock::now() + std::chrono::milliseconds(5),
            [&logger]() {
              return !logger.queue_.empty() || !logger.running_.load();
            });
      }

      for (;;) {
        Info info;
        {
          std::lock_guard lock(logger.mutex_);
          if (logger.queue_.empty()) {
            break;
          }
          info = std::move(logger.queue_.front());
          logger.queue_.pop();
        }

        logger.file_ << std::format(
                            "{:%Y-%m-%d %H:%M:%S} |{: <6}| {} | {}:{}:{}",
                            info.time,
                            leval_string[static_cast<int>(info.level)],
                            info.message, info.location.file_name(),
                            info.location.line(), info.location.function_name())
                     << std::endl;
      }

      if (!logger.running_.load()) {
        return;
      }
    }
  });

  logger.initialized_.store(true);
}

void Logger::Log(Level level,
                 std::string message,
                 const std::source_location& location,
                 NowTime time) {
  Logger& logger = Logger::Get();
  if (level < logger.level_) {
    return;
  }

  {
    std::unique_lock lock(logger.mutex_);
    logger.queue_.emplace(level, std::move(message), location, std::move(time));
  }
  logger.cv_.notify_one();
}

Logger::~Logger() {
  Logger& logger = Logger::Get();
  logger.running_.store(false);
  logger.cv_.notify_one();

  if (logger.thread_.joinable()) {
    logger.thread_.join();
  }

  logger.file_.flush();
  logger.file_.close();
}
Logger::Logger() : level_(Level::kInfo), initialized_(false) {}

}  // namespace base
