#pragma once

#include <memory>
#include <sstream>

#include <v8.h>

#include "api.hpp"
#include "from-disposable.hpp"
#include "transform.hpp"

namespace braid::vm {

static inline void throwFrom(v8::TryCatch& tryCatch) {
  throw std::runtime_error(toString(tryCatch.Message()->Get()));
}

static v8::Local<v8::Value> executeInIsolation(const std::string& js) {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

  auto isolate = v8::Isolate::New(params);

  v8::Isolate::Scope isolationScope(isolate);

  v8::HandleScope scope(isolate);

  auto context = v8::Context::New(isolate);

  v8::Context::Scope contextScope(context);

  api::ObjectTemplateFactory::create(isolate, context->Global());

  v8::TryCatch tryCatch;

  auto script = v8::Script::Compile(context, local(js));

  if (tryCatch.HasCaught()) {
    throwFrom(tryCatch);
  }

  v8::Local<v8::Value> result = script.ToLocalChecked()->Run();

  if (tryCatch.HasCaught()) {
    throwFrom(tryCatch);
  }

  return result;
}

} // namespace braid::vm