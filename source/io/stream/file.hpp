#pragma once

#include <cassert>
#include <codecvt>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/locale.hpp>

#include "../../structures/conditional-failure.hpp"

#include "transform.hpp"

namespace braid::stream {

inline void conditional_failure(const boost::filesystem::path& path, const std::function<bool()> failed) {
  if (failed) {
    throw std::system_error(errno, std::system_category(), path.c_str());
  }
}

template<typename CharT = char, typename Traits = std::char_traits<CharT>>
inline std::basic_string<CharT, Traits> read(const boost::filesystem::path& path) {
  boost::filesystem::basic_ifstream<CharT, Traits> buf(path, std::ios_base::in);

  buf.open();

  structures::ConditionalFailure(path.c_str(), [&]() {
    return buf.fail();
  });

  return boost::locale::conv::from_utf<CharT>(streamToString(buf));
}

template<>
inline std::basic_string<char, std::char_traits<char>> read<char, std::char_traits<char>>(const boost::filesystem::path& path) {
  boost::filesystem::basic_ifstream<char, std::char_traits<char>> buf(path, std::ios_base::in);

  structures::ConditionalFailure(path.c_str(), [&]() {
    return buf.fail();
  });

  return streamToString(buf);
}

template<typename CharT = wchar_t, typename Traits = std::char_traits<CharT>>
inline void write(const boost::filesystem::path& path, const std::basic_string<CharT, Traits>& string) {
  boost::filesystem::ofstream buf(path, std::ios_base::out);

  structures::ConditionalFailure(path.c_str(), [&]() {
    return buf.fail();
  });

  buf << boost::locale::conv::to_utf<char>(string);
}

template<>
inline void write(const boost::filesystem::path& path, const std::basic_string<char, std::char_traits<char>>& string) {
  boost::filesystem::ofstream buf(path, std::ios_base::out);

  structures::ConditionalFailure(path.c_str(), [&]() {
    return buf.bad();
  });

  buf << string;
}

} // namespace braid::stream