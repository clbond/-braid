#pragma once

#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <thread>

#include <boost/any.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>

#include "post-wait.h"

namespace braid::thread {
  class controlled_thread : private boost::noncopyable {
    public:
      controlled_thread()
        : service_(new boost::asio::io_service())
      {}

      virtual ~controlled_thread() {
        join();
      }

      void start() {
        auto fn = [=]() {
          service_->run();

          {
            std::lock_guard<std::mutex> guard(mutex_);

            std::for_each(complete_.begin(), complete_.end(),
              [](std::function<void()> complete) {
                complete();
              });

            complete_.clear();
          }
        };

        thread_.reset(new std::thread(fn));
      }

      void stop() {
        service_->stop();
      }

      void join() {
        if (thread_) {
          thread_->join();
          thread_.reset();
        }
      }

      template<typename ResultT>
      void task(std::promise<ResultT>& promise, std::function<ResultT()> task) {
        post_and_wait(service_, promise, task);
      }

      void complete(std::function<void()> completed) {
        std::lock_guard<std::mutex> guard(mutex_);

        complete_.push_back(completed);
      }

    private:
      mutable std::mutex mutex_;

      std::shared_ptr<boost::asio::io_service> service_;

      std::shared_ptr<std::thread> thread_;

      std::vector<std::function<void()>> complete_;
  };
}