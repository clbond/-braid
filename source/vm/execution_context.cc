#include <v8.h>

#include "buffer_allocator.hpp"
#include "execution_context.hpp"
#include "from_disposable.hpp"
#include "transform.hpp"

using namespace braid;
using namespace braid::vm;
using namespace std;

static shared_ptr<v8::Isolate> newIsolate() {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = new buffer_allocator();

  return shared_from_disposable(v8::Isolate::New(params));
}

execution_context::execution_context()
  : isolation(newIsolate()),
    v8context(v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>>(isolation.get(), v8::Context::New(isolation.get())))
{}

shared_ptr<v8::Isolate> execution_context::isolate() const {
  return isolation;
}

execution_context::context_type& execution_context::context() const {
  return v8context;
}

v8::Local<v8::Value> execution_context::execute(const v8::Local<v8::String> js) {
  v8::Locker locker(isolation.get());

  v8::Isolate::Scope isolatescope(isolation.get());

  auto context = v8::Local<v8::Context>::New(isolation.get(), v8context);

  auto script = v8::Script::Compile(context, js).ToLocalChecked();

  return script->Run();
}

v8::Local<v8::Context> execution_context::current_context() const {
  v8::HandleScope scope(isolation.get());

  return isolation->GetCurrentContext();
}
