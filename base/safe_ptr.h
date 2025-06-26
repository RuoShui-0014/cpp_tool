#pragma once

namespace base {

template <typename T>
class safe_ptr {
 public:
  explicit safe_ptr(T* ptr) : ptr_(ptr) {}
  ~safe_ptr() { ptr_ = nullptr; }

  T* operator->() const {
    return ptr_;
  }

 private:
  T* ptr_;
};

}  // namespace base
