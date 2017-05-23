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

      void run(std::promise<ReturnT>& promise) {
        const auto init = !thread_;
        if (init) {
          thread_ = thread_pool_->start_controlled();
        }
        thread_->task(promise, fn_);
        if (init) {
          thread_->start();
        }
      }

      virtual ~execution() {
        if (thread_) {
          thread_->join();
          thread_.reset();
        }
      }

    private:
      std::shared_ptr<braid::thread::pool> thread_pool_;

      std::shared_ptr<controlled_thread> thread_;

      std::function<ReturnT()> fn_;
  };
}