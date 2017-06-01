#pragma once

#include <memory>

#include <v8.h>

namespace braid::vm {

inline std::shared_ptr<v8::Isolate> createIsolate() {
  v8::Isolate::CreateParams params;

  auto allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

  params.array_buffer_allocator = allocator;

  return std::shared_ptr<v8::Isolate>(v8::Isolate::New(params),
    [=](v8::Isolate* impl) {
      impl->Dispose();

      delete allocator;
    });
}

} // namespace braid::vm