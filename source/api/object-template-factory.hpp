#pragma once

#include <v8.h>

#include "../vm/types.hpp"

namespace braid::api {

struct ObjectTemplateFactory {
  static v8::Local<v8::ObjectTemplate> create(vm::IsolatePtr isolation);
};

} // namespace braid::api