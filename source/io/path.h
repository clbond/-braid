#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace braid::path {
  template<typename CharT = char, typename Traits = std::char_traits<CharT>>
  inline const std::vector<boost::filesystem::path> vector(const std::vector<std::basic_string<CharT, Traits>>& paths) {
    std::vector<boost::filesystem::path> v;

    std::transform(paths.cbegin(), paths.cend(), std::back_inserter(v),
      [&](const std::basic_string<CharT, Traits>& s) {
        return boost::filesystem::path(s);
      });

    return v;
  }
}