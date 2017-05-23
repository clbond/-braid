#pragma once

#include <future>

#include <boost/asio/io_service.hpp>

namespace braid::thread {
  template<typename T>
  inline std::shared_future<T> post_and_wait(std::shared_ptr<boost::asio::io_service> service, std::function<T()> fn) {
    std::promise<T> promise;

    service->post(
      [=]() -> void {
        try {
          promise.set_value(fn());
        }
        catch (const std::exception&) {
          promise.set_exception(std::current_exception());
        }
      });

    return promise.get_future();
  }

  template<>
  inline std::shared_future<void> post_and_wait(std::shared_ptr<boost::asio::io_service> service, std::function<void()> fn) {
    std::promise<void> promise;

    service->post(
      [=, &promise]() -> void {
        try {
          fn();
          promise.set_value();
        }
        catch (const std::exception&) {
          promise.set_exception(std::current_exception());
        }
      });

    return promise.get_future();
  }
}