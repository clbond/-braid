#pragma once

#include <future>

#include <boost/asio/io_service.hpp>

namespace braid::thread {
  template<
    typename T,
    typename = boost::disable_if_c<std::is_void<T>::value>> // use resultless for void
  class task {
    public:
      task(boost::asio::io_service& service, std::shared_ptr<std::promise<T>> promise, std::function<std::shared_future<T>()> fn)
        : task(service, promise, fn, task_result::shared_future)
      {}

      task(boost::asio::io_service& service, std::shared_ptr<std::promise<T>> promise, std::function<std::future<T>()> fn)
        : task(service, promise, fn, task_result::future)
      {}

      task(boost::asio::io_service& service, std::shared_ptr<std::promise<T>> promise, std::function<T()> fn)
        : task(service, promise, fn, task_result::value)
      {}

      void operator()() {
        try {
          switch (result_) {
            case task_result::future:
            case task_result::shared_future:
              promise_->set_value(fn_().get());
              break;
            case task_result::value:
              promise_->set_value(fn_());
              break;
            default:
              throw std::runtime_error("Invalid task_result enum value");
          }
        }
        catch (const std::exception&) {
          promise_->set_exception(std::current_exception());
        }
      }

    protected:
      enum task_result {future, shared_future, value};

      task(boost::asio::io_service& service, std::shared_ptr<std::promise<T>> promise, std::function<T()> fn, const task_result result_type)
        : promise_(promise), fn_(fn), result_(result_type), work_(service)
      {}

    private:
      std::shared_ptr<std::promise<T>> promise_;

      std::function<T()> fn_;

      task_result result_;

      boost::asio::io_service::work work_;
  };

  class resultless_task {
    public:
      resultless_task(boost::asio::io_service& service, std::shared_ptr<std::promise<void>> promise, std::function<void()> fn)
        : promise_(promise), fn_(fn), work_(service)
      {}

      void operator()() {
        try {
          fn_();

          promise_->set_value();
        }
        catch (const std::exception&) {
          promise_->set_exception(std::current_exception());
        }
      }

    private:
      std::shared_ptr<std::promise<void>> promise_;

      std::function<void()> fn_;

      boost::asio::io_service::work work_;
  };
}