#include <v8.h>

#include "buffer_allocator.hpp"
#include "execution_context.hpp"
#include "from_disposable.hpp"
#include "transform.hpp"

using namespace braid;
using namespace braid::vm;
using namespace std;

static v8::Isolate* newIsolate() {
  v8::Isolate::CreateParams params;
  //params.array_buffer_allocator = new buffer_allocator();

  return v8::Isolate::New(params);
}

execution_context::execution_context()
  : isolation(newIsolate())
{}

v8::Isolate* execution_context::isolate() const {
  return isolation;
}

v8::Local<v8::Value> execution_context::execute(const v8::Local<v8::String> js) {
  v8::Isolate::Scope isolation_scope(isolation);

  v8::HandleScope scope(isolation);

  auto context = v8::Context::New(isolation, NULL, v8::ObjectTemplate::New(isolation), js);

  v8::HandleScope handle_scope(isolation);

  v8::Persistent<v8::Context> persist(isolation, context);

  v8::Context::Scope context_scope(context);

  auto script = v8::Script::Compile(context, js).ToLocalChecked();

  cout << "Received script" << &script << endl;

  return script->Run(context).ToLocalChecked();
}
