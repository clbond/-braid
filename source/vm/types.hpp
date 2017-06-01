#pragma once

#include <v8.h>

namespace braid::vm {

typedef v8::Isolate* IsolatePtr;

template<typename T>
using CopyablePersistent = v8::Persistent<T, v8::CopyablePersistentTraits<T>>;

} // namespace braid::vm