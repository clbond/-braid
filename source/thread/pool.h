#pragma once

#include <algorithm>
#include <cstdint>
#include <future>
#include <functional>
#include <iostream>
#include <thread>
#include <set>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>

#include "controlled.h"
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
        while (workers-- > 0) {
          create();
        }

        std::vector<std::shared_ptr<std::thread>> threads;
        std::copy(threads_.begin(), threads_.end(), std::back_inserter(threads));

        return threads;
      }

      std::shared_ptr<controlled_thread> start_controlled() {
        std::shared_ptr<controlled_thread> thread(new controlled_thread());

        thread->complete(
          [&]() {
            auto iterator = controlled_threads_.find(thread);
            if (iterator != controlled_threads_.end()) {
              controlled_threads_.erase(iterator);
            }
          });

        controlled_threads_.insert(thread);

        return thread;
      }

      template<typename ResultT>
      std::shared_future<ResultT> task(std::promise<ResultT>& promise, std::function<ResultT()> task) {
        post_and_wait(service_, promise, task);
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

        std::for_each(controlled_threads_.begin(), controlled_threads_.end(),
          [](std::shared_ptr<controlled_thread> thread) {
            thread->join();
          });

        controlled_threads_.clear();
      }

    protected:
      std::shared_ptr<std::thread> create() {
        std::shared_ptr<std::thread> thread(new std::thread(boost::bind(&boost::asio::io_service::run, service_)));

        threads_.insert(thread);

        return thread;
      }

    private:
      std::shared_ptr<boost::asio::io_service> service_;

      std::set<std::shared_ptr<controlled_thread>> controlled_threads_;

      std::set<std::shared_ptr<std::thread>> threads_;
  };
}