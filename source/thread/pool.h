#pragma once

#include <algorithm>
#include <cstdint>
#include <future>
#include <functional>
#include <thread>
#include <vector>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>

#include "post-wait.h"

namespace braid::thread {
  class pool : private boost::noncopyable {
    public:
      pool()
        : service_(new boost::asio::io_service())
      {}

      virtual ~pool() {
        join();
      }

      std::vector<std::shared_ptr<std::thread>> start_workers(std::size_t workers) {
        std::vector<std::shared_ptr<std::thread>> v;

        while (workers-- > 0) {
          v.push_back(create());
        }

        return v;
      }

      template<typename ResultT>
      std::shared_future<ResultT> task(std::function<ResultT()> task) {
        return post_and_wait(service_, task);
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

    protected:
      std::shared_ptr<std::thread> create() {
        std::shared_ptr<std::thread> thread(new std::thread(boost::bind(&boost::asio::io_service::run, service_)));

        threads_.push_back(thread);

        return thread;
      }

    private:
      std::shared_ptr<boost::asio::io_service> service_;

      std::vector<std::shared_ptr<std::thread>> threads_;
  };
}