#pragma once

#include <future>
#include <iostream>
#include <type_traits>

#include <boost/asio/io_service.hpp>

#include "types.hpp"

namespace braid::thread {
  template<typename T>
  void set_promise_from_function(std::shared_ptr<std::promise<T>> promise, std::function<T()> fn) {
    promise->set_value(fn());
  }

  template<>
  void set_promise_from_function(std::shared_ptr<std::promise<void>> promise, std::function<void()> fn) {
    fn();
    promise->set_value();
  }

  template<typename T>
  void set_promise_from_function(std::shared_ptr<std::promise<T>> promise, std::function<std::shared_future<T>()> fn) {
    promise->set_value(fn.get());
  }

  template<>
  void set_promise_from_function(std::shared_ptr<std::promise<void>> promise, std::function<std::shared_future<void>()> fn) {
    fn().get();
    promise->set_value();
  }

  struct task {
    virtual void operator()() = 0;
  };

  template<typename T>
  class task_with_promise : public task {
    public:
      task_with_promise(service& service, std::shared_ptr<std::promise<T>> promise, std::function<T()> fn)
        : promise_(promise), fn_(fn), work_(service)
      {}

      void operator()() {
        try {
          set_promise_from_function(promise_, fn_);
        }
        catch (const std::exception& ex) {
          promise_->set_exception(std::current_exception());
        }
      }

    private:
      std::shared_ptr<std::promise<T>> promise_;

      std::function<T()> fn_;

      service::work work_; // prevent service from stopping until the task finishes
  };
}
