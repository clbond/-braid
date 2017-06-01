#include <stdexcept>
#include <sstream>

namespace braid::exceptions {
  class Exception : public std::runtime_error {
    public:
      template<typename CharT = char, typename Traits = std::char_traits<CharT>>
      Exception(const std::basic_string<CharT, Traits>& msg)
        : std::runtime_error(msg)
      {}

      Exception(const char* msg)
        : std::runtime_error(std::string(msg))
      {}
  };

  class ScriptException : public Exception {
    public:
      ScriptException(const std::string& msg)
        : Exception(msg)
      {}
  };

  class CommandLineException : public Exception {
    public:
      CommandLineException(const std::string& msg)
        : Exception(msg)
      {}
  };

  class NotImplementedException : public Exception {
    public:
      NotImplementedException()
        : Exception("Not implemented")
      {}
  };
}
