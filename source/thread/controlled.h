#pragma once

#include <future>
#include <memory>
#include <thread>

#include <boost/asio/io_service.hpp>

#include <boost/noncopyable.hpp>

#include "post-wait.h"

namespace braid::thread {
  template<typename ReturnT>
  class controlled_thread : private boost::noncopyable {
    public:
      controlled_thread(std::function<ReturnT()> fn)
        : fn_(std::move(fn))
      {}

      virtual ~controlled_thread() {
        join();
      }

      std::shared_future<ReturnT> complete() const {
        return fn_.get_future();
      }

      template<typename T>
      std::shared_future<T> task(std::function<T()> fn) {
        return post_and_wait(service_, fn);
      }

      void start() {
        service_.run();
      }

      void stop() {
        service_.stop();
      }

      void join() {
        if (thread_) {
          thread_->join();
          thread_.reset();
        }
      }

    private:
      boost::asio::io_service service_;

      std::function<ReturnT()> fn_;

      std::shared_ptr<std::thread> thread_;
  };
}