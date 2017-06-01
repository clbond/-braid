#pragma once

#include <memory>
#include <sstream>

#include <v8.h>

#include "api.hpp"
#include "execution.hpp"
#include "from-disposable.hpp"
#include "isolate.hpp"
#include "transform.hpp"
#include "types.hpp"

namespace braid::vm {

class ScriptRunner : public ExecutionEnvironment {
  public:
    ScriptRunner() = default;
    ScriptRunner(const ScriptRunner&) = delete;

    virtual void execute(const std::string& identifier, const std::string& source);

  private:
    std::shared_ptr<v8::Isolate> mIsolate = createIsolate();
};

} // namespace braid::vm