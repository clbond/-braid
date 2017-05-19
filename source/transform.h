#include <sstream>
#include <string>

namespace braid::transform {
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