#include <stdexcept>

namespace braid::exceptions {
  class command_line_error : public std::runtime_error {
    public:
      command_line_error(const std::string& msg)
        : runtime_error(msg)
      {}
  };

  class not_implemented : public std::runtime_error {
    public:
      not_implemented()
        : runtime_error("Not implemented")
      {}
  };
}
