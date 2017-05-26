#pragma once

#include <string>
#include <vector>
#include <utility>

#include <JavaScriptCore/JavaScriptCore.h>

#include <boost/filesystem/path.hpp>
#include <boost/locale.hpp>

namespace braid::stream::transform {
  template<typename CharT, typename Traits = std::char_traits<CharT>>
  inline const std::basic_string<CharT, Traits>&& string(const std::basic_istream<CharT, Traits>& stream) {
    std::basic_stringstream<CharT, Traits> ss;
    ss << stream.rdbuf();

    return std::move(ss.str());
  }

  template<typename CharT, typename Traits = std::char_traits<CharT>>
  inline const JSStringRef js_string(const std::basic_string<CharT, Traits>& string) {
    return JSStringCreateWithUTF8CString(boost::locale::conv::to_utf<char>(string));
  }

  template<typename CharT, typename Traits = std::char_traits<CharT>>
  inline const JSStringRef js_string(const std::basic_istream<CharT, Traits>& stream) {
    std::basic_stringstream<CharT, Traits> ss;
    ss << stream.rdbuf();

    return js_string(ss.str());
  }

  template<>
  inline const JSStringRef js_string(const std::basic_string<char, std::char_traits<char>>& string) {
    return JSStringCreateWithUTF8CString(string.c_str());
  }
}