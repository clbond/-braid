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
#include <boost/asio/io_service.hpp>

#include "task.hpp"

namespace braid::thread {
  class dispatcher {
    public:
      dispatcher()
        : service_(new service())
      {}

      dispatcher(const dispatcher&) = delete;

      dispatcher(dispatcher&&) = delete;

      std::vector<std::shared_ptr<std::thread>> start_workers(std::size_t workers) {
        while (workers-- > 0) {
          threads_.push_back(std::shared_ptr<std::thread>(
            new std::thread(boost::bind(&service::run, service_))));
        }
        return threads_;
      }

      template<typename T>
      std::shared_future<T> dispatch(std::function<std::shared_future<T>()> fn) {
        std::shared_ptr<std::promise<T>> promise(new std::promise<T>());

        service_->dispatch(task_with_promise<T>(*service_, promise,
          [=]() {
            return fn().get();
          }));

        return promise->get_future();
      }

      template<typename T>
      std::shared_future<T> dispatch(std::function<T()> fn) {
        std::shared_ptr<std::promise<T>> promise(new std::promise<T>());

        service_->dispatch(task_with_promise<T>(*service_, promise, fn));

        return promise->get_future();
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

      virtual ~dispatcher() {
        join();
      }

    private:
      std::shared_ptr<service> service_;

      std::vector<std::shared_ptr<std::thread>> threads_;
  };

}