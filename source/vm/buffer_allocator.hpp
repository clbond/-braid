#pragma once

#include <cstdlib>

#include <v8.h>

namespace braid::vm {

// struct buffer_allocator : virtual v8::ArrayBuffer::Allocator {
//   buffer_allocator() {}

//   virtual ~buffer_allocator() {}

//   virtual void* Allocate(size_t length) {
//     auto block = std::malloc(length);
//     if (block != nullptr) {
//       std::memset(block, 0, length);
//     }
//     return block;
//   }

//   virtual void* AllocateUninitialized(size_t length) {
//     return std::malloc(length);
//   }

//   virtual void Free(void* data, size_t length) {
//     std::free(data);
//   }
// };

} // namespace braid::vm