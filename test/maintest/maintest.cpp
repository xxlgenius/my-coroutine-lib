#include <chrono>
#include <coroutine>
#include <iostream>
#include <string>

void print(std::string str) {
  std::cout << str << std::endl;
}

struct PerviewAwaiter {
  bool await_ready() const noexcept { return false; }
  std::coroutine_handle<> await_suspend(
      std::coroutine_handle<> handle) const noexcept {
    if (mperHandle) {
      return mperHandle;
    } else {
      return std::noop_coroutine();
    }
  }
  void await_resume() const noexcept {}

  std::coroutine_handle<> mperHandle = nullptr;
};

template <typename T>
struct Promise {
  auto initial_suspend() noexcept { return std::suspend_always{}; }
  auto final_suspend() noexcept { return PerviewAwaiter(mperview); }
  auto unhandled_exception() noexcept {
    mexception = std::current_exception();
    // throw;
  }
  auto yield_value(T v) {
    new (&mvalue) T(std::move(v));
    return std::suspend_always();
  }
  auto get_return_object() noexcept {
    return std::coroutine_handle<Promise>::from_promise(*this);
  }
  void return_value(T v) noexcept { new (&mvalue) T(std::move(v)); }

  T result() {
    if (mexception) [[unlikely]] {
      std::rethrow_exception(mexception);
    }
    T res = std::move(mvalue);
    mvalue.~T();
    return res;
  }
  union {
    T mvalue;
  };

  Promise() noexcept {}
  Promise(Promise&&) = delete;
  ~Promise() {}

  std::coroutine_handle<> mperview = nullptr;
  std::exception_ptr mexception = nullptr;
};

template <>
struct Promise<void> {
  auto initial_suspend() noexcept { return std::suspend_always{}; }
  auto final_suspend() noexcept { return PerviewAwaiter(mperview); }
  auto unhandled_exception() noexcept {
    mexception = std::current_exception();
    // throw;
  }

  auto get_return_object() noexcept {
    return std::coroutine_handle<Promise>::from_promise(*this);
  }
  void return_void() noexcept {}
  void result() {
    if (mexception) [[unlikely]] {
      std::rethrow_exception(mexception);
    }
    return;
  }

  Promise() noexcept {}
  Promise(Promise&&) = delete;
  ~Promise() {}

  std::coroutine_handle<> mperview = nullptr;
  std::exception_ptr mexception = nullptr;
};

template <typename T>
struct Task {
  using promise_type = Promise<T>;
  Task(std::coroutine_handle<promise_type> handle) : mTaskHandle(handle) {}
  ~Task() { mTaskHandle.destroy(); }
  Task(Task const&) = delete;

  struct Awaiter {
    bool await_ready() const noexcept { return false; }
    std::coroutine_handle<> await_suspend(
        std::coroutine_handle<> handle) const noexcept {
      mhandle.promise().mperview = handle;  // 保存调用者的句柄
      return mhandle;
    }
    T await_resume() const noexcept { return mhandle.promise().result(); }

    std::coroutine_handle<promise_type> mhandle = nullptr;
  };
  auto operator co_await() const noexcept { return Awaiter{mTaskHandle}; }
  std::coroutine_handle<promise_type> mTaskHandle = nullptr;  // 当前协程的句柄
};

Task<void> task2() {
  print("aaa");
  // throw std::runtime_error("task2 error");
  co_return ;
}

Task<std::string> task1() {
  co_await (task2());
  print("task1 得到2");
  co_return "bbb";
}

int main() {
  Task t = task1();
  while (!t.mTaskHandle.done()) {
    t.mTaskHandle.resume();
    print("main 获得值：" + t.mTaskHandle.promise().result());
  }
  return 0;
}