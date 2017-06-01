#pragma once

#include <v8.h>

#include "../types.hpp"

namespace braid::vm::api {

class Console {
  public:
    static v8::Local<v8::ObjectTemplate> create(IsolatePtr);
};

} // namespace braid::vm::api