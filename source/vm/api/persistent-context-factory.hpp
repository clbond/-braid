#pragma once

#include <v8.h>

namespace braid::vm::api {

struct PersistentContextFactory {
  static v8::Local<v8::ObjectTemplate> create(std::shared_ptr<v8::Isolate>);
};

} // namespace braid::api