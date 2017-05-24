#pragma once

#include <algorithm>
#include <cstdint>
#include <future>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>

#include "task.h"

namespace braid::thread {
  class dispatcher : private boost::noncopyable {
    public:
      dispatcher()
        : service_(new boost::asio::io_service())
      {}

      virtual ~dispatcher() {
        join();
      }

      std::shared_ptr<boost::asio::io_service> service() {
        return service_;
      }

      std::shared_ptr<const boost::asio::io_service> service() const {
        return std::const_pointer_cast<const boost::asio::io_service>(service_);
      }

      template<typename T>
      std::shared_future<T> dispatch(std::function<T()> fn) {
        std::shared_ptr<std::promise<T>> promise(new std::promise<T>());

        service_->dispatch(task<T>(*service_, promise, fn));

        return promise->get_future();
      }

      std::vector<std::shared_ptr<std::thread>> start_workers(std::size_t workers) {
        while (workers-- > 0) {
          threads_.push_back(std::shared_ptr<std::thread>(
            new std::thread(boost::bind(&boost::asio::io_service::run, service_))));
        }
        return threads_;
      }

      void stop() {
        service_->stop();
      }

      void join() {
        std::for_each(threads_.begin(), threads_.end(),
          [](std::shared_ptr<std::thread> thread) {
            thread->join();
          });

        threads_.clear();
      }

    private:
      std::shared_ptr<boost::asio::io_service> service_;

      std::vector<std::shared_ptr<std::thread>> threads_;
  };

  template<>
  std::shared_future<void> dispatcher::dispatch(std::function<void()> fn) {
    std::shared_ptr<std::promise<void>> promise(new std::promise<void>());

    service_->dispatch(resultless_task(*service_, promise, fn));

    return promise->get_future();
  }
}