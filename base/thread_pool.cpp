#include "thread_pool.h"

#include <thread>

namespace base {

ThreadPool::ThreadPool() = default;
ThreadPool::~ThreadPool() {
  running_.store(false);
  if (server_.joinable()) {
    server_.join();
  }
}

ThreadPool* ThreadPool::Get() {
  static ThreadPool thread_pool;
  return &thread_pool;
}

void ThreadPool::Start(Params params) {
  params_ = params;

  running_.store(true);
  server_ = std::thread([this]() {
    while (running_.load()) {
      {
        std::unique_lock lock(exec_mutex_);
        cv_.wait(lock, [this]() { return !running_.load(); });
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
    threads_ins_.emplace_back(std::make_unique<ThreadIns>(i, queue_mutex_));
  }
}

ThreadPool::ThreadIns::ThreadIns(int id, std::mutex& mutex)
    : queue_mutex_(mutex) {
  running_.store(true);
  worker_ = std::thread([this, id]() {
    while (running_.load() || TaskCount()) {
      {
        std::unique_lock exec_lock(exec_mutex_);
        cv_.wait(exec_lock,
                 [this]() { return TaskCount() || !running_.load(); });
      }

      std::function<void()> task;
      {
        std::lock_guard queue_lock(queue_mutex_);
        if (!queue_.empty()) {
          task = queue_.front();
          queue_.pop();
        }
      }
      if (task) {
        task();
      }
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
