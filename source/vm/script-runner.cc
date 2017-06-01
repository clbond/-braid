#include "execution.hpp"
#include "isolate.hpp"
#include "script-runner.hpp"
#include "try-catch.hpp"

using namespace braid;
using namespace braid::vm;
using namespace std;
using namespace v8;

void ScriptRunner::execute(const std::string& identifier, const std::string& source) {
  v8::Isolate::Scope isolateScope(mIsolate.get());

  v8::HandleScope hscope(mIsolate.get());

  v8::EscapableHandleScope scope(mIsolate.get());

  v8::TryCatch tryCatch(mIsolate.get());

  auto objectTemplate = api::PersistentContextFactory::create(mIsolate);

  auto context = v8::Context::New(mIsolate.get(), nullptr, objectTemplate);

  v8::Context::Scope contextScope(context);

  auto script = v8::Script::Compile(context, local(source));

  throwIfCaught(tryCatch, "Compile: " + identifier);

  script.ToLocalChecked()->Run();

  throwIfCaught(tryCatch, identifier);
}
