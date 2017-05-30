#include "object-template-factory.hpp"

using namespace braid;
using namespace braid::vm::api;
using namespace braid::vm;
using namespace std;
using namespace v8;

#include "../transform.hpp"

#include "console.hpp"

void ObjectTemplateFactory::create(IsolatePtr isolate, Local<Object> t) {
  t->Set(String::NewFromUtf8(isolate, "console"), Console::create(isolate));
}
