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

    v8::Isolate* isolate() const;

    template<typename CharT = char, typename Traits = std::char_traits<CharT>>
    v8::Local<v8::Value> execute(const std::basic_string<CharT, Traits>& js) {
      return execute(transform(isolation, js));
    }

    v8::Local<v8::Value> execute(const v8::Local<v8::String> string);

  private:
    mutable v8::Isolate* isolation;

    v8::Persistent<v8::Context> context_;

    v8::Persistent<v8::ObjectTemplate> global_;
};

} // namespace braid::vm