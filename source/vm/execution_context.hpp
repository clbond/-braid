#pragma once

#include <future>
#include <memory>

#include <v8.h>

#include "transform.hpp"

namespace braid::vm {

class execution_context {
  class scope;

  public:
    explicit execution_context();

    execution_context(const execution_context&) = delete;

    execution_context(execution_context&&) = delete;

    std::shared_ptr<v8::Isolate> isolate() const;

    typedef v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>> context_type;

    context_type& context() const;

    template<typename CharT = char, typename Traits = std::char_traits<CharT>>
    v8::Local<v8::Value> execute(const std::basic_string<CharT, Traits>& js) {
      return execute(transform(js));
    }

    v8::Local<v8::Value> execute(const v8::Local<v8::String> string);

    v8::Local<v8::Context> current_context() const;

  private:
    mutable std::shared_ptr<v8::Isolate> isolation;

    mutable context_type v8context;
};

class execution_context::scope {
  public:
    scope() = delete;
    scope(const scope&) = delete;
    scope(scope&&) = delete;

    explicit scope(std::shared_ptr<execution_context> c) {
      // context->Enter();
    }

    ~scope() {
      // context->Exit();
    }

  private:
};

} // namespace braid::vm