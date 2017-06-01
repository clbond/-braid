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

Local<ObjectTemplate> Console::create(IsolatePtr isolate) {
  EscapableHandleScope scope(isolate);

  Local<ObjectTemplate> object = ObjectTemplate::New(isolate);

  object->Set(String::NewFromUtf8(isolate, "log"),
    FunctionTemplate::New(isolate,
      [](const FunctionCallbackInfo<Value>& args) {
        cout << joinArgs(args) << endl;
      }));

  object->Set(String::NewFromUtf8(isolate, "error"),
    FunctionTemplate::New(isolate,
      [](const FunctionCallbackInfo<Value>& args) {
        cerr << joinArgs(args) << endl;
      }));

   return scope.Escape(object);
}