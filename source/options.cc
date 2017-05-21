#include <algorithm>
#include <iostream>
#include <vector>

#include <boost/program_options.hpp>

#include "exception.h"
#include "io.h"
#include "options.h"

using namespace braid;
using namespace std;

namespace po = boost::program_options;

shared_ptr<const options>
braid::parse_command_line_arguments(po::command_line_parser& parser) {
  po::options_description desc("Options");

  desc.add_options()
      ("help", "produce description of available options")
      ("source-file", po::value<vector<string>>(), "JavaScript source files")
      ("debug", po::value<bool>()->default_value(false), "enable application debugging")
      ("workers", po::value<std::size_t>()->default_value(8), "number of thread pool workers to create")
      ;

  po::positional_options_description positional;
  positional.add("source-file", -1);

  po::variables_map vm;
  po::store(parser.options(desc).positional(positional).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    stringstream ss;
    ss << desc;
    throw exceptions::command_line_error(ss.str());
  }

  if (vm.count("source-file") == 0) {
    throw exceptions::command_line_error("No source files were specified");
  }

  auto result = shared_ptr<options>(new options());

  result->debug = vm.count("debug") > 0;
  result->workers = vm["workers"].as<std::size_t>();
  result->entries = braid::path::vector(vm["source-file"].as<vector<string>>());

  return const_pointer_cast<const options>(result);
}
