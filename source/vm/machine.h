#pragma once

#include <future>

#include <JavaScriptCore/JSContext.h>

namespace braid::vm {
  struct script;

  struct machine {
    template<typename CharT = char, typename Traits = std::char_traits<CharT>>
    std::shared_ptr<script> compile(const std::basic_string<CharT, Traits>& content) {
      return std::shared_ptr<script>();
    }

    std::shared_future<void> execute(std::shared_ptr<script> script) {
      return std::shared_future<void>();
    }
  };
}