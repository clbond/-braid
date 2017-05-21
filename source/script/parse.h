#pragma once

#include <iostream>
#include <memory>

#include "runnable.h"

namespace braid::script {
  template<typename CharT = char, typename Traits = std::char_traits<CharT>>
  std::shared_ptr<runnable> parse(const std::basic_string<CharT, Traits>& content) {
    std::cout << "parse: [" << content << "]" << std::endl;

    return std::shared_ptr<runnable>(new runnable());
  }
}