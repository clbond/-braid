#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include <boost/bind.hpp>

#include <boost/utility/enable_if.hpp>

namespace braid::vm {

template<typename T>
void dispose(T* impl) {
  impl->Dispose();
}

template<typename Pointer, typename T>
Pointer fromDisposable(T* impl) {
  return Pointer(impl,
    [](T* p) {
      dispose(p);
    });
}

template<typename T>
std::shared_ptr<T> sharedFromDisposable(T* impl) {
  return fromDisposable<std::shared_ptr<T>, T>(impl);
}

template<typename T>
inline std::unique_ptr<T> uniqueFromDisposable(T* impl) {
  return fromDisposable<std::unique_ptr<T>, T>(impl);
}

} // namespace braid::vm