#pragma once

#include <future>

#include <boost/noncopyable.hpp>

#include "../thread.h"

namespace braid::script {
  class runnable : private boost::noncopyable {
    public:
      std::future<void> run(std::shared_ptr<braid::thread_pool> thread_pool) const {
        std::packaged_task<void()> runner_task(
          [&]() {
            // TODO(bond): Execute JS
          });

        return thread_pool->task(std::move(runner_task));
      }
  };
}