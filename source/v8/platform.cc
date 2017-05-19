#include <memory>

#include "platform.h"

using namespace braid::v8;
using namespace std;

platform::platform()
  : platform_(::v8::platform::CreateDefaultPlatform())
{
  ::v8::V8::InitializePlatform(platform_.get());
  ::v8::V8::Initialize();
}

platform::~platform() {
  platform_.reset();

  ::v8::V8::Dispose();
  ::v8::V8::ShutdownPlatform();
}
