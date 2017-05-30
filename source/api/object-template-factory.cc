#include "object-template-factory.hpp"

using namespace braid;
using namespace braid::api;
using namespace braid::vm;
using namespace v8;

Local<ObjectTemplate> ObjectTemplateFactory::create(IsolatePtr isolate) {
  auto objtemplate = v8::ObjectTemplate::New(isolate);

  return objtemplate;
}
