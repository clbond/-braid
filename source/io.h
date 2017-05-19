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

#include "transform.h"
#include "unconstructable.h"

namespace braid::io {
  namespace file {
    template<typename CharT = char, typename Traits = std::char_traits<CharT>>
    inline std::string read(const boost::filesystem::path& path) {
      boost::filesystem::basic_ifstream<CharT, Traits> buf(path, std::ios_base::in);

      std::wstring_convert<std::codecvt_utf8<CharT>, CharT> convert; // handle utf8 -> wchar_t

      return convert.from_bytes(transform::streamToString(buf));
    }

    template<typename CharT = char, typename Traits = std::char_traits<CharT>>
    inline void write(const boost::filesystem::path& path, const std::basic_string<CharT, Traits>& string) {
      boost::filesystem::ofstream buf(path, std::ios_base::out);

      std::wstring_convert<std::codecvt_utf8<char>, char> convert; // write utf8

      buf << convert.from_bytes(string);
    }
  }

  namespace transform {
    template<typename CharT, typename Traits>
    inline const std::basic_string<CharT, Traits> streamToString(const std::basic_filebuf<CharT, Traits>& stream) {
      std::basic_stringstream<CharT, Traits> ss;

      ss << stream.rdbuf();

      return ss.str();
    }

    template<typename CharT, typename Traits>
    inline const std::basic_string<CharT, Traits> streamToString(const std::basic_istream<CharT, Traits>& stream) {
      std::basic_stringstream<CharT, Traits> ss;

      ss << stream;

      return ss.str();
    }
  }
}