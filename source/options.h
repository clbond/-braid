#pragma once

#include <memory>
#include <vector>
#include <string>

#include <boost/filesystem/path.hpp>

#include <boost/program_options.hpp>

namespace braid {
  struct options {
    // Paths to entrypoint JavaScript code which will be executed in parallel
    std::vector<boost::filesystem::path> entries;

    // Are we running in debug mode?
    bool debug = false;
  };

  std::shared_ptr<const options> parse_command_line_arguments(boost::program_options::command_line_parser& parser);
}