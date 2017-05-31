#pragma once

#include <future>
#include <type_traits>

#include <boost/asio/io_service.hpp>

#include "types.hpp"

namespace braid::thread {

struct Task {
  virtual void operator()() = 0;
};

template<typename T>
class TaskWithPromise : public Task {
  public:
    TaskWithPromise(Service& service, std::shared_ptr<std::promise<T>> promise, std::function<T()> fn)
      : mPromise(promise), mFunction(fn), mWork(service)
    {}

    void operator()() {
      try {
        setPromiseFromFunction(mPromise, mFunction);
      }
      catch (const std::exception& ex) {
        mPromise->set_exception(std::current_exception());
      }
    }

  private:
    std::shared_ptr<std::promise<T>> mPromise;

    std::function<T()> mFunction;

    Service::work mWork; // prevent service from stopping until the task finishes
};

} // namespace braid::vm