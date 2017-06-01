#pragma once

#include <v8.h>

#include "types.hpp"

namespace braid::vm {

struct ExecutionEnvironment {
  // Execute the specified JavaScript code in the current thread in a new isolation
  virtual void execute(const std::string& identifier, const std::string& source) = 0;
};

} // namespace braid::vm