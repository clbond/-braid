#pragma once

#include <algorithm>
#include <cstdint>
#include <future>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>

#include "promise.hpp"

#include "task.hpp"

namespace braid::thread {
  class Dispatcher {
    public:
      Dispatcher()
        : mService(new Service())
      {}

      Dispatcher(const Dispatcher&) = delete;

      Dispatcher(Dispatcher&&) = delete;

      std::vector<std::shared_ptr<std::thread>> start_workers(std::size_t workers) {
        while (workers-- > 0) {
          mThreads.push_back(std::shared_ptr<std::thread>(new std::thread(boost::bind(&Service::run, mService))));
        }
        return mThreads;
      }

      template<typename T, typename F>
      std::shared_future<T> dispatch(std::shared_ptr<std::promise<T>> promise, F fn) {
        mService->dispatch(TaskWithPromise<T>(*mService, promise, fn));

        return promise->get_future();
      }

      void stop() {
        mService->stop();
      }

      void join() {
        std::for_each(mThreads.begin(), mThreads.end(),
          [](std::shared_ptr<std::thread> thread) {
            thread->join();
          });

        mThreads.clear();
      }

      virtual ~Dispatcher() {
        join();
      }

    private:
      std::shared_ptr<Service> mService;

      std::vector<std::shared_ptr<std::thread>> mThreads;
  };

}