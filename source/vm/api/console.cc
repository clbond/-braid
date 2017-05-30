#include <iostream>
#include <vector>

#include <boost/algorithm/string/join.hpp>

#include "console.hpp"

#include "../transform.hpp"

using namespace braid;
using namespace braid::vm;
using namespace braid::vm::api;
using namespace std;
using namespace v8;

static inline const std::string joinArgs(const FunctionCallbackInfo<Value>& args) {
  vector<string> v;
  for (auto j = 0; j < args.Length(); ++j) {
    v.push_back(toString(args[j]));
  }
  return boost::algorithm::join(v, " ");
}

Handle<Object> Console::create(IsolatePtr isolate) {
  Handle<Object> object = Object::New(isolate);

  auto fn = FunctionTemplate::New(isolate,
    [](const FunctionCallbackInfo<Value>& args) {
      cout << joinArgs(args) << endl;
    });

  object->Set(String::NewFromUtf8(isolate, "log"), fn->GetFunction());

  return object;
}