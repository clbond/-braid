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

  template<typename Output, typename Input>
  inline const std::vector<Output> map(const std::vector<Input>& vector, const std::unary_function<Input, Output> f) {
    std::vector<boost::filesystem::path> v;

    std::transform(vector.begin(), vector.end(), std::back_inserter(v),
      [=](const Input& s) -> Output {
        return f(s);
      });

    return v;
  }

  template<typename CharT = char, typename Traits = std::char_traits<CharT>>
  inline const std::vector<boost::filesystem::path> strings_to_paths(const std::vector<std::basic_string<CharT, Traits>>& paths) {
    return map(paths,
      [](const std::string& s) {
        return boost::filesystem::path(s);
      });
  }
}
