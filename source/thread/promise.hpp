#pragma once

#include <future>
#include <memory>

namespace braid::thread {

template<typename T>
void setPromiseFromFunction(std::shared_ptr<std::promise<T>> promise, std::function<T()> fn) {
  promise->set_value(fn());
}

template<>
void setPromiseFromFunction(std::shared_ptr<std::promise<void>> promise, std::function<void()> fn) {
  fn();
  promise->set_value();
}

template<typename T>
void setPromiseFromFunction(std::shared_ptr<std::promise<T>> promise, std::function<std::shared_future<T>()> fn) {
  promise->set_value(fn.get());
}

template<>
void setPromiseFromFunction(std::shared_ptr<std::promise<void>> promise, std::function<std::shared_future<void>()> fn) {
  fn().get();
  promise->set_value();
}

template<typename T, typename F>
void setPromiseFromFunction(std::shared_ptr<std::promise<T>> promise, F&& fn) {
  promise->set_value(fn());
}

typedef void (*voidfn)();

template<>
void setPromiseFromFunction(std::shared_ptr<std::promise<void>> promise, voidfn&& fn) {
  fn();
  promise->set_value();
}

} // namespace braid::thread