#pragma once

#include <v8.h>

#include "../../vm/types.hpp"

namespace braid::vm::api {

struct ObjectTemplateFactory {
  static void create(vm::IsolatePtr, v8::Local<v8::Object>);
};

} // namespace braid::api