#pragma once

#include <algorithm>
#include <future>
#include <functional>
#include <memory>
#include <thread>
#include <set>

#include <boost/bind.hpp>

#include <boost/asio/io_service.hpp>

namespace braid {
  class thread_pool {
    public:
      template<typename CharT = char, typename Traits = std::char_traits<CharT>>
      std::shared_ptr<std::thread> create(const std::basic_string<CharT, Traits>& code) {
        std::shared_ptr<std::thread> thread(new std::thread(boost::bind(&boost::asio::io_service::run, service_)));

        threads_.insert(thread);

        return thread;
      }

      thread_pool()
        : service_(new boost::asio::io_service())
      {}

      template<typename MessageT, typename ResultT>
      std::future<ResultT> task(const std::packaged_task<ResultT>&& task) {
        service_->post(std::bind(&std::packaged_task<ResultT>::operator(), task));

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

    private:
      std::shared_ptr<boost::asio::io_service> service_;

      std::set<std::shared_ptr<std::thread>> threads_;
  };
}