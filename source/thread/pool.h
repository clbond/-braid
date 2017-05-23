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

#include "async.h"

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

      template<typename ResultT>
      void task(std::promise<ResultT>& promise, std::function<ResultT()> task) {
        async_execute(service_, promise, task);
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

        threads_.insert(thread);

        return thread;
      }

    private:
      std::shared_ptr<boost::asio::io_service> service_;

      std::set<std::shared_ptr<std::thread>> threads_;
  };
}