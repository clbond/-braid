#pragma once

#include <stdexcept>
#include <sstream>

#include <v8.h>

#include "transform.hpp"

namespace braid::vm {

template<typename CharT = char, typename Traits = std::char_traits<CharT>>
static inline void throwIfCaught(v8::TryCatch& catcher, const std::basic_string<CharT, Traits>& msg = std::basic_string<CharT, Traits>()) {
  if (catcher.HasCaught() == false) {
    return;
  }

  std::basic_stringstream<CharT, Traits> ss(msg);

  const std::string delimiter(": ");

  if (msg.size()) {
    ss << delimiter;
  }

  if (catcher.HasTerminated()) {
    ss << "Execution has been terminated" << delimiter;
  }
  else {
    ss << "Uncaught exception" << delimiter;
  }

  ss << toString(catcher.Message()->Get())
    << std::endl
    << toString(catcher.StackTrace(v8::Isolate::GetCurrent()->GetCurrentContext()).ToLocalChecked());

  throw std::runtime_error(ss.str());
}

} // namespace braid::vm