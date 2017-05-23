#pragma once

#include <future>
#include <iostream>

#include <boost/asio/io_service.hpp>

namespace braid::thread {
  template<typename T>
  void async_execute(std::shared_ptr<boost::asio::io_service> service, std::promise<T>& promise, std::function<std::shared_future<T>()> fn) {
    service->post(
      [=, &promise]() -> void {
        try {
          std::shared_future<T> future = fn();

          if (future.valid() == false) {
            promise.set_exception(std::make_exception_ptr(new std::runtime_error("Task function returned an invalid future")));
          }

          promise.set_value(future.get()); // wait is alright because the task runs on a separate thread
        }
        catch (const std::exception&) {
          promise.set_exception(std::current_exception());
        }
      });
  }

  template<typename T>
  void async_execute(std::shared_ptr<boost::asio::io_service> service, std::promise<T>& promise, std::function<T()> fn) {
    service->post(
      [=, &promise]() -> void {
        try {
          promise.set_value(fn());
        }
        catch (const std::exception&) {
          promise.set_exception(std::current_exception());
        }
      });
  }

  template<>
  void async_execute(std::shared_ptr<boost::asio::io_service> service, std::promise<void>& promise, std::function<void()> fn) {
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
  }
}