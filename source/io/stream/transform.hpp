#pragma once

#include <string>
#include <vector>
#include <utility>

#include <boost/filesystem/path.hpp>
#include <boost/locale.hpp>

namespace braid::stream {

template<typename CharT = char, typename Traits = std::char_traits<CharT>>
inline const std::basic_string<CharT, Traits> streamToString(const std::basic_istream<CharT, Traits>& stream) {
  std::basic_stringstream<CharT, Traits> ss;
  ss << stream.rdbuf();

  return ss.str();
}

} // braid::stream::transform