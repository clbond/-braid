#include <iostream>

#include "console-delegate.hpp"
#include "persistent-context-factory.hpp"

using namespace braid;
using namespace braid::vm::api;
using namespace braid::vm;
using namespace std;
using namespace v8;

Local<ObjectTemplate> PersistentContextFactory::create(shared_ptr<Isolate> isolate) {
  v8::EscapableHandleScope handleScope(isolate.get());

  auto context = v8::Context::New(isolate.get());

  v8::Context::Scope contextScope(context);

  debug::SetConsoleDelegate(isolate.get(), new ConsoleDelegate(isolate));

  auto objtemplate = ObjectTemplate::New(isolate.get());

  objtemplate->SetImmutableProto();

  return handleScope.Escape(objtemplate);
}
