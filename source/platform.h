#pragma once

#include <v8.h>

#include <libplatform/libplatform.h>

namespace braid::v8 {
  class execution_context;

  class platform {
    public:
      platform();
      platform(const platform&) = delete;

      platform& operator=(const platform&) = delete;

      std::shared_ptr<execution_context> createExecutionContext() const;

      ~platform();

    private:
      std::shared_ptr<::v8::Platform> platform_;
  };
}
