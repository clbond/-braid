#include <memory>

#include "execution-context.h"
#include "platform.h"

using namespace braid::v8;
using namespace std;

platform::platform()
  : platform_(::v8::platform::CreateDefaultPlatform())
{
  ::v8::V8::InitializePlatform(platform_.get());
  ::v8::V8::Initialize();
}

shared_ptr<execution_context> platform::createExecutionContext() const {
  return shared_ptr<execution_context>(new execution_context());
}

platform::~platform() {
  platform_.reset();

  ::v8::V8::Dispose();
  ::v8::V8::ShutdownPlatform();
}
