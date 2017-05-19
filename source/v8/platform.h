#pragma once

#include <v8.h>

#include <libplatform/libplatform.h>

namespace braid::v8 {
  class platform {
    public:
      platform();
      platform(const platform&) = delete;

      platform& operator=(const platform&) = delete;

      ~platform();

    private:
      std::shared_ptr<::v8::Platform> platform_;
  };
}
