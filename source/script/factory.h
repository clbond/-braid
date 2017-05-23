#pragma once

#include <iostream>
#include <future>
#include <memory>

#include <boost/noncopyable.hpp>

#include "../thread.h"

namespace braid::script {
  template<typename CharT = char, typename Traits = std::char_traits<CharT>>
  class executable_code : public std::unary_function<std::shared_ptr<braid::thread::pool>, void> {
    public:
      executable_code(const std::basic_string<CharT, Traits>& code)
        : code_(code)
      {}

      void operator()(std::shared_ptr<braid::thread::pool> pool) {
        std::function<void()> fn([&]() {
          std::cout << "Executing: " << code_ << std::endl;
        });

        pool->task(promise_, fn);
      }

      std::shared_future<void> get_future() {
        return promise_.get_future();
      }

    private:
      const std::basic_string<CharT, Traits> code_;

      std::promise<void> promise_;
  };

  class executable_code_factory : private boost::noncopyable {
    public:
      template<typename CharT = char, typename Traits = std::char_traits<CharT>>
      std::shared_ptr<executable_code<CharT, Traits>> from_string(const std::basic_string<CharT, Traits> content) {
        return std::shared_ptr<executable_code<CharT, Traits>>(new executable_code<CharT, Traits>(content));
      }
  };
}