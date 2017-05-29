#pragma once

#include <cstdlib>

#include <v8.h>

namespace braid::vm {

class buffer_allocator : public v8::ArrayBuffer::Allocator {
  virtual void* Allocate(size_t length) {
    auto block = std::malloc(length);
    if (block != nullptr) {
      std::memset(block, 0, length);
    }
    return block;
  }

  virtual void* AllocateUninitialized(size_t length) {
    return std::malloc(length);
  }

  virtual void Free(void* data, size_t length) {
    std::free(data);
  }
};

} // namespace braid::vm