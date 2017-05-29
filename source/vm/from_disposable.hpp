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

template<typename T>
std::shared_ptr<T> shared_from_disposable(T* impl) {
  return std::shared_ptr<T>(impl,
    [](T* p) {
      dispose(p);
    });
}

template<typename T>
inline std::unique_ptr<T> unique_from_disposable(T* impl) {
  return std::unique_ptr<T>(impl,
    [](T* p) {
      dispose(p);
    });
}

} // namespace braid::vm