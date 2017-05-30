#include <v8.h>

#include "execution_context.hpp"
#include "from_disposable.hpp"
#include "transform.hpp"

using namespace braid;
using namespace braid::vm;
using namespace std;

static shared_ptr<v8::Isolate> newIsolate() {
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

  return shared_from_disposable(v8::Isolate::New(params));
}

execution_context::execution_context()
  : isolation(newIsolate())
{}

v8::Local<v8::Value> execution_context::execute(const string& js) {
  auto isolate = isolation.get();

  v8::Isolate::Scope isolation_scope(isolate);

  v8::HandleScope scope(isolate);

  auto context = v8::Context::New(isolate, NULL, v8::ObjectTemplate::New(isolate));

  v8::HandleScope handle_scope(isolate);

  v8::Persistent<v8::Context> persist(isolate, context);

  v8::Context::Scope context_scope(context);

  v8::Local<v8::String> source = transform(isolate, js);

  auto script = v8::Script::Compile(context, source).ToLocalChecked();

  cout << "Received script" << &script << endl;

  return script->Run(context).ToLocalChecked();
}
