#pragma once

#include <iostream>
#include <memory>

#include <boost/noncopyable.hpp>

#include "execution.h"

#include "../thread.h"

namespace braid::script {
  class execution_factory : private boost::noncopyable {
    public:
      execution_factory(std::shared_ptr<braid::thread::pool> thread_pool)
        : thread_pool_(thread_pool)
      {}

      template<typename T = void>
      std::shared_ptr<execution<T>> from_function(std::function<T()> function) {
        return std::shared_ptr<execution<T>>(new execution<T>(thread_pool_, function));
      }

      template<typename T = void, typename CharT = char, typename Traits = std::char_traits<CharT>>
      std::shared_ptr<execution<T>> from_string(const std::basic_string<CharT, Traits> content) {
        return from_function<T>(
          [=]() {
            std::cerr << "Parse script: " << content << std::endl;

            // TODO(cbond): Execute script
            return T();
          });
      }

    private:
      std::shared_ptr<braid::thread::pool> thread_pool_;
  };
}