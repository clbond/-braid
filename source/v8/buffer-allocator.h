#pragma once

#include <cstdlib>
#include <cstdint>

#include <v8.h>

namespace braid::v8 {
  struct buffer_allocator : public ::v8::ArrayBuffer::Allocator {
    virtual void* Allocate(std::size_t length) {
      return std::malloc(length);
    }

    virtual void* AllocateUninitialized(std::size_t length) {
      return std::malloc(length);
    }

    virtual void Free(void* data, const std::size_t length) {
      std::free(data);
    }
  };
}