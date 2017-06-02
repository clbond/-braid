#include <iostream>
#include <vector>

#include <boost/algorithm/string/join.hpp>

#include "console-delegate.hpp"

#include "../transform.hpp"
#include "../types.hpp"
#include "../try-catch.hpp"

using namespace braid;
using namespace braid::vm;
using namespace braid::vm::api;
using namespace std;
using namespace v8;

template<typename CharT = char, typename Traits = std::char_traits<CharT>>
static basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const debug::ConsoleCallArguments& args) {
  v8::TryCatch tryCatch(Isolate::GetCurrent());

  vector<string> arguments;

  for (auto j = 0; j < args.Length(); ++j) {
    Local<Value> arg = args[j];

    if (args[j]->IsSymbol()) {
      arg = Local<Symbol>::Cast(arg)->Name();
    }

    Local<String> stringValue;

    if (!arg->ToString(Isolate::GetCurrent()->GetCurrentContext()).ToLocal(&stringValue)) {
      Isolate::GetCurrent()->ThrowException(local("Failed to serialize argument"));
      break;
    }

    if (conditionalThrowToVm(tryCatch)) {
      break;
    }
    else {
      arguments.push_back(toString(stringValue));
    }
  }

  return os << boost::algorithm::join(arguments, " ");
}

ConsoleDelegate::ConsoleDelegate(shared_ptr<Isolate> isolate)
  : mIsolate(isolate)
{}

void ConsoleDelegate::Log(const debug::ConsoleCallArguments& args) {
  cout << args << endl;
}

void ConsoleDelegate::Error(const debug::ConsoleCallArguments& args) {
  cerr << args << endl;
}

void ConsoleDelegate::Warn(const debug::ConsoleCallArguments& args) {
  cerr << "Warning: " << args << endl;
}

void ConsoleDelegate::Trace(const v8::debug::ConsoleCallArguments& args) {
}

void ConsoleDelegate::Assert(const v8::debug::ConsoleCallArguments& args) {
  if (args.Length() < 1) {
    return;
  }

  bool assertionValue =
    args[0]->IsBoolean()
      ? args[0].As<v8::Boolean>()->Value()
      : args[0]->BooleanValue(mIsolate->GetCurrentContext()).FromMaybe(true);

  if (assertionValue == false) {
    stringstream ss;
    ss << "Assertion failed: " << assertionValue;

    if (args.Length() > 1) {
      ss << ": " << toString(args[1]);
    }

    mIsolate->ThrowException(local(ss.str()));
  }
}

void ConsoleDelegate::Info(const debug::ConsoleCallArguments& args) {
  cout << args << endl;
}

void ConsoleDelegate::Debug(const debug::ConsoleCallArguments& args) {
}

void ConsoleDelegate::Time(const debug::ConsoleCallArguments& args) {
  // if (args.Length() == 0) {
  //   default_timer_ = base::TimeTicks::HighResolutionNow();
  // } else {
  //   Local<Value> arg = args[0];
  //   Local<String> label;
  //   v8::TryCatch try_catch(isolate_);
  //   if (!arg->ToString(isolate_->GetCurrentContext()).ToLocal(&label)) {
  //     Shell::ReportException(isolate_, &try_catch);
  //     return;
  //   }
  //   v8::String::Utf8Value utf8(label);
  //   std::string string(*utf8);
  //   auto find = timers_.find(string);
  //   if (find != timers_.end()) {
  //     find->second = base::TimeTicks::HighResolutionNow();
  //   } else {
  //     timers_.insert(std::pair<std::string, base::TimeTicks>(
  //         string, base::TimeTicks::HighResolutionNow()));
  //   }
  // }
}

void ConsoleDelegate::TimeEnd(const debug::ConsoleCallArguments& args) {
  // base::TimeDelta delta;
  // base::TimeTicks now = base::TimeTicks::HighResolutionNow();
  // if (args.Length() == 0) {
  //   delta = base::TimeTicks::HighResolutionNow() - default_timer_;
  //   printf("default: ");
  // } else {
  //   Local<Value> arg = args[0];
  //   Local<String> label;
  //   v8::TryCatch try_catch(isolate_);
  //   if (!arg->ToString(isolate_->GetCurrentContext()).ToLocal(&label)) {
  //     Shell::ReportException(isolate_, &try_catch);
  //     return;
  //   }
  //   v8::String::Utf8Value utf8(label);
  //   std::string string(*utf8);
  //   auto find = timers_.find(string);
  //   if (find != timers_.end()) {
  //     delta = now - find->second;
  //   }
  //   printf("%s: ", *utf8);
  // }
  // printf("%f\n", delta.InMillisecondsF());
}
