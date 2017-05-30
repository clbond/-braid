#pragma once

#include <future>
#include <memory>

#include <v8.h>

#include "transform.hpp"

namespace braid::vm {

class execution_context {
  public:
    execution_context();
    execution_context(const execution_context&) = delete;
    execution_context(execution_context&&) = delete;

    v8::Local<v8::Value> execute(const std::string& code);

  private:
    mutable std::shared_ptr<v8::Isolate> isolation;

    v8::Persistent<v8::Context> context_;

    v8::Persistent<v8::ObjectTemplate> global_;
};

} // namespace braid::vm