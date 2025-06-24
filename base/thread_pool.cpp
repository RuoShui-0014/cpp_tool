#include "thread_pool.h"

#include <thread>
#include "logger.h"

namespace base {

ThreadPool::ThreadPool() = default;
ThreadPool::~ThreadPool() {
  Logger::Log(Logger::Level::kInfo, "Will close thread pool.");

  running_.store(false);
  if (server_.joinable()) {
    server_.join();
  }
  Logger::Log(Logger::Level::kInfo, "Thread pool close success.");
}

ThreadPool* ThreadPool::Get() {
  static ThreadPool thread_pool;
  return &thread_pool;
}

void ThreadPool::Start(Params params) {
  Logger::Log(Logger::Level::kInfo, "Thread pool start.");

  params_ = params;

  running_.store(true);
  server_ = std::thread([this]() {
    while (running_.load()) {
      {
        std::unique_lock lock(exec_mutex_);
        cv_.wait_until(
            lock,
            std::chrono::system_clock::now() + std::chrono::milliseconds(10),
            [this]() { return !running_.load(); });
      }
    }


    for (auto& ins : threads_ins_) {
      ins->Stop();
    }
    for (auto& ins : threads_ins_) {
      ins->Wait();
    }
  });

  for (int i = 0; i < params_.min_threads; i++) {
    threads_ins_.emplace_back(std::make_unique<ThreadIns>(i));
  }
}

ThreadPool::ThreadIns::ThreadIns(int id): id_(id) {
  Logger::Log(Logger::Level::kInfo, std::format("Thread pool`s child {} start.", id_));

  running_.store(true);
  worker_ = std::thread([this, id]() {
    while (running_.load() || TaskCount() > 0) {
      {
        std::unique_lock lock(exec_mutex_);
        cv_.wait(lock, [this]() { return TaskCount() || !running_.load(); });
      }

      std::function<void()> task;
      {
        std::lock_guard lock(queue_mutex_);
        if (!queue_.empty()) {
          task = std::move(queue_.front());
          queue_.pop();
        } else {
          continue;
        }
      }
      task();
    }
  });
}

ThreadPool::ThreadIns::~ThreadIns() = default;

void ThreadPool::ThreadIns::Stop() {
  running_.store(false);
  cv_.notify_all();
}

void ThreadPool::ThreadIns::Wait() {
  worker_.join();
}

}  // namespace base
