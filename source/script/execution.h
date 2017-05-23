#pragma once

#include <functional>
#include <future>

#include <boost/noncopyable.hpp>

#include "../thread.h"

namespace braid::script {
  using braid::thread::controlled_thread;

  template<typename ReturnT = void>
  class execution : private boost::noncopyable {
    public:
      execution(std::shared_ptr<braid::thread::pool> thread_pool, std::function<ReturnT()> fn)
        : thread_pool_(thread_pool), fn_(fn)
      {}

      std::shared_ptr<controlled_thread<ReturnT>> run() const {
        return std::shared_ptr<controlled_thread<ReturnT>>(new controlled_thread<ReturnT>(fn_));
      }

    private:
      std::shared_ptr<braid::thread::pool> thread_pool_;

      std::function<ReturnT()> fn_;
  };
}