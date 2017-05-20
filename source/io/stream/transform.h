#pragma once

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace braid::stream::transform {
  template<typename CharT, typename Traits = std::char_traits<CharT>>
  inline const std::basic_string<CharT, Traits> string(const std::basic_istream<CharT, Traits>& stream) {
    std::basic_stringstream<CharT, Traits> ss;

    ss << stream.rdbuf();

    return ss.str();
  }

}
