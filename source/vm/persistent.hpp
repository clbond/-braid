#pragma once

#include <v8.h>

namespace braid::vm {

template<typename T>
class persistent {
  public:
    explicit persistent(v8::Handle<T> handle)
      : handle(v8::Persistent<T>::New(handle))
    {}

    ~persistent() {
      dispose();
    }

    inline operator bool() const {
      return handle.IsEmpty() == false;
    }

    inline v8::Persistent<T> operator->() const {
      return handle;
    }

    bool empty() const {
      return handle.IsEmpty();
    }

    void dispose() {
      if (!empty()) {
        handle.Dispose();
        handle.Clear();
      }
    }

  private:
    v8::Persistent<T> handle;
};

} // namespace braid::vm