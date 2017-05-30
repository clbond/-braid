#pragma once

#include <v8.h>

#include <string>

#include "../types.hpp"

namespace braid::vm::api {

template<typename T, typename... Args>
v8::Handle<v8::Object> wrapObject(IsolatePtr isolate, v8::Handle<v8::Object> target, Args&&... args) {
  auto instance = new T(std::forward(args)...);

  target->SetInternalField(0, v8::External::New(instance));

	return target;
}

} // namespace braid::vm::api