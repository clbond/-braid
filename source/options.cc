#include <iostream>
#include <vector>

#include <boost/program_options.hpp>

#include "exception.h"
#include "options.h"

using namespace braid;
using namespace std;

namespace po = boost::program_options;

shared_ptr<const Options> Options::parse(const int argc, const char** argv) {
  po::options_description desc("Options");

  desc.add_options()
      ("help", "produce description of available options")
      ("input-file", po::value<vector<string>>(), "JavaScript source files")
      ("debug", po::value<bool>()->default_value(false), "enable application debugging");

  po::positional_options_description positional;
  positional.add("input-file", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(positional).run(), vm);
  po::notify(vm);

  if (vm.count("input-file") == 0) {
    throw braid::exceptions::command_line_error("No source files were specified");
  }

  if (vm.count("help")) {
    stringstream ss;
    ss << desc;
    throw exceptions::command_line_error(ss.str());
  }

  auto options = shared_ptr<Options>(new Options());
  options->debug = vm.count("debug") > 0;
  options->entries = vm["input-file"].as<vector<string>>();

  return options;
}