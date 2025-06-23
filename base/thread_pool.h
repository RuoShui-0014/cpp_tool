#pragma once

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

#include "macro.h"

namespace base {

class BASE_EXPORT ThreadPool {
 public:
  struct Params {
    Params() = default;
    Params(unsigned int max_threads, unsigned int min_threads)
        : max_threads(max_threads), min_threads(min_threads) {}
    unsigned int max_threads;
    unsigned int min_threads;
  };

  ThreadPool();
  ~ThreadPool();

  static ThreadPool* Get();

  void Start(Params params = {std::thread::hardware_concurrency(), 1});

  template <typename Func, typename... Args>
  void AddTask(Func&& task, Args&&... args) {
    std::vector<int> data(threads_ins_.size());
    for (int i = 0; i < data.size(); i++) {
      data[i] = threads_ins_[i]->TaskCount();
    }
    auto min_it = std::ranges::min_element(data);
    for (int i = 0; i < data.size(); i++) {
      if (data.at(i) == *min_it) {
        threads_ins_[i]->AddTask(std::forward<Func>(task),
                                 std::forward<Args>(args)...);
        return;
      }
    }
  }

 private:
  class ThreadIns {
   public:
    explicit ThreadIns(int id, std::mutex& mutex);
    ~ThreadIns();

    void Stop();
    void Wait();
    int TaskCount() {
      int count = 0;
      {
        std::lock_guard lock(queue_mutex_);
        count = static_cast<int>(queue_.size());
      }
      return count;
    }

    template <typename Func, typename... Args>
    void AddTask(Func&& task, Args&&... args) {
      {
        std::lock_guard lock(queue_mutex_);
        queue_.emplace(
            std::bind(std::forward<Func>(task), std::forward<Args>(args)...));
      }
      cv_.notify_all();
    }

   private:
    std::mutex exec_mutex_;
    std::condition_variable cv_;
    std::mutex& queue_mutex_;

    std::thread worker_;
    std::atomic_bool running_;
    std::queue<std::function<void()>> queue_;
  };

  std::mutex queue_mutex_;
  std::mutex exec_mutex_;
  std::condition_variable cv_;

  Params params_;
  std::thread server_;
  std::atomic_bool running_;
  std::vector<std::unique_ptr<ThreadIns>> threads_ins_;
};

}  // namespace base
