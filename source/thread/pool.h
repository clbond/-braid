#pragma once

#include <algorithm>
#include <cstdint>
#include <future>
#include <functional>
#include <thread>
#include <vector>

#include <boost/bind.hpp>

#include <boost/asio/io_service.hpp>

namespace braid {
  class thread_pool {
    public:
      thread_pool()
        : service_(new boost::asio::io_service())
      {}

      virtual ~thread_pool() {
        join();
      }

      void start_workers(std::size_t workers) {
        while (workers-- > 0) {
          create();
        }
      }

      template<typename ResultT>
      std::future<ResultT> task(std::packaged_task<ResultT()>&& task) {
        service_->post([&]() {
          task();
        });

        return task.get_future();
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
      template<typename CharT = char, typename Traits = std::char_traits<CharT>>
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