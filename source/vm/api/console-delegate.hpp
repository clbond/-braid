#pragma once

#include <memory>

#include <v8.h>
#include <v8-debug.h>
#include <v8-inspector.h>

#include <debug/debug-interface.h>

#include "../types.hpp"

namespace braid::vm::api {

class ConsoleDelegate : public v8::debug::ConsoleDelegate {
  public:
    ConsoleDelegate(std::shared_ptr<v8::Isolate> isolate);

    void Debug(const v8::debug::ConsoleCallArguments& args) override;
    void Error(const v8::debug::ConsoleCallArguments& args) override;
    void Info(const v8::debug::ConsoleCallArguments& args) override;
    void Log(const v8::debug::ConsoleCallArguments& args) override;
    void Warn(const v8::debug::ConsoleCallArguments& args) override;
    void Trace(const v8::debug::ConsoleCallArguments& args) override;
    void Assert(const v8::debug::ConsoleCallArguments& args) override;
    void Time(const v8::debug::ConsoleCallArguments& args) override;
    void TimeEnd(const v8::debug::ConsoleCallArguments& args) override;

    virtual ~ConsoleDelegate() = default;

  private:
    std::shared_ptr<v8::Isolate> mIsolate;
};

} // namespace braid::vm::api
