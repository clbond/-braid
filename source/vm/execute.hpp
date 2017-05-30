#pragma once

#include <memory>
#include <sstream>

#include <v8.h>

#include "from_disposable.hpp"

#include "transform.hpp"

namespace braid::vm {

static inline void conditional_throw(v8::TryCatch& tryCatch, const std::string& msg) {
  if (tryCatch.HasCaught()) {
    std::stringstream ss;
    ss << msg << ": " << to_string(tryCatch.Message()->Get());

    throw std::runtime_error(ss.str());
  }
}

static v8::Local<v8::Value> execute_in_isolation(const std::string& js) {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

  auto isolate = v8::Isolate::New(params);

  v8::Isolate::Scope isolation_scope(isolate);

  v8::HandleScope scope(isolate);

  auto context = v8::Context::New(isolate);

  v8::Context::Scope context_scope(context);

  v8::TryCatch tc;

  auto script = v8::Script::Compile(context, transform(isolate, js));

  conditional_throw(tc, "Script compilation failed");

  auto result = script.ToLocalChecked()->Run();

  conditional_throw(tc, "Script execution failed");

  return result;
}


} // namespace braid::vm