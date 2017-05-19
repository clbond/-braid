#include <memory>
#include <string>

#include "buffer-allocator.h"
#include "exception.h"
#include "execution-context.h"

using namespace braid::v8;
using namespace std;

v8::MaybeLocal<v8::Value> execution_context::run(const string& scriptSource) {
  throw braid::exceptions::not_implemented();
}

void execution_context::join() {}