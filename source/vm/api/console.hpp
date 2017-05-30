#pragma once

#include <v8.h>

#include "../types.hpp"

namespace braid::vm::api {

class Console {
  public:
    static v8::Handle<v8::Object> create(IsolatePtr);
};

} // namespace braid::vm::api