#pragma once

#include <boost/thread.hpp>

#include <v8.h>

namespace braid::v8 {
  class execution_context {
    public:
      ::v8::MaybeLocal<::v8::Value> run(const std::string& scriptSource);

      void join();

    private:
      boost::thread_group mThreadGroup;
  };
}