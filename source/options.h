#pragma once

#include <memory>
#include <vector>
#include <string>

#include <boost/program_options.hpp>

#include "stream.h"

namespace braid {
  struct options;

  std::shared_ptr<const options> parse_cli(boost::program_options::command_line_parser& parser);

  struct options {
    // Paths to entrypoint JavaScript code which will be executed in parallel
    braid::path_vector entries;

    // Are we running in debug mode?
    bool debug = false;
  };
}