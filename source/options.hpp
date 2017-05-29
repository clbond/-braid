#pragma once

#include <memory>
#include <vector>
#include <string>

#include <boost/filesystem/path.hpp>

#include <boost/program_options.hpp>

namespace braid {
  class options {
    public:
      options(const std::size_t workers, const bool debug)
        : workers_(workers), debug_(debug)
      {}

      const std::vector<boost::filesystem::path>& entries() const {
        return entries_;
      }

      std::vector<boost::filesystem::path>& entries() {
        return entries_;
      }

      const std::size_t workers() const {
        return workers_;
      }

      const bool debug() const {
        return debug_;
      }

    private:
      std::vector<boost::filesystem::path> entries_;
      std::size_t workers_;
      bool debug_;
  };

  std::shared_ptr<const options> parse_command_line_arguments(boost::program_options::command_line_parser& parser);
}