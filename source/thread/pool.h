#pragma once

#include <algorithm>
#include <memory>

#include <boost/thread/thread.hpp>

#include "thread.h"

namespace braid {
  class thread_pool {
    public:
      template<typename CharT = char, typename Traits = std::char_traits<CharT>>
      std::shared_ptr<braid::thread> create(const std::basic_string<CharT, Traits>& code) {
        std::shared_ptr<braid::thread> thread(new braid::thread());

        thread_pointers_.push_back(thread);

        boost::asio::io_service& service = thread->service();

        threads_.create_thread(boost::bind(&boost::asio::io_service::run, &service));

        return thread;
      }

      void join() {
        std::for_each(thread_pointers_.begin(), thread_pointers_.end(),
          [](const std::shared_ptr<braid::thread> thread) {
            thread->service().stop();
          });

        threads_.join_all();

        thread_pointers_.clear();
      }

    private:
      std::vector<std::shared_ptr<braid::thread>> thread_pointers_;

      boost::thread_group threads_;
  };
}