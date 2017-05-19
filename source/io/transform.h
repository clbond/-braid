#pragma once

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace braid::transform {
  template<typename CharT, typename Traits = std::char_traits<CharT>>
  inline const std::basic_string<CharT, Traits> stream_to_string(const std::basic_istream<CharT, Traits>& stream) {
    std::basic_stringstream<CharT, Traits> ss;

    ss << stream.rdbuf();

    return ss.str();
  }

  template<typename CharT = char, typename Traits = std::char_traits<CharT>>
  inline const std::vector<boost::filesystem::path> strings_to_paths(const std::vector<std::basic_string<CharT, Traits>>& paths) {
    std::vector<boost::filesystem::path> v;

    std::transform(paths.cbegin(), paths.cend(), std::back_inserter(v),
      [&](const std::basic_string<CharT, Traits>& s) {
        return boost::filesystem::path(s);
      });

    return v;
  }
}
