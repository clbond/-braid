#pragma once

#include <JavaScriptCore/JSBase.h>
#include <JavaScriptCore/JSContext.h>

#include <future>

namespace braid::vm {
  class machine {
    public:
      machine()
        : root_(JSContextGroupCreate())
      {}

      JSGlobalContextRef context() {
        return JSGlobalContextCreateInGroup(root_, nullptr);
      }

    private:
      JSContextGroupRef root_;
  };
}