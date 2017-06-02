#include <algorithm>
#include <functional>
#include <sstream>
#include <thread>
#include <vector>

#include <boost/program_options.hpp>

#include "algorithms.hpp"
#include "exception.hpp"
#include "io.hpp"
#include "options.hpp"

using namespace braid;
using namespace std;
using namespace boost::program_options;

template<typename T>
inline T argumentValue(const variables_map& vm, const string& argument) {
  return vm[argument].as<T>();
}

template<>
inline bool argumentValue(const variables_map& vm, const string& argument) {
  return vm.count(argument) > 0;
}

shared_ptr<const Options> Options::parseCommandLine(command_line_parser& parser) {
  options_description desc("Options");

  desc.add_options()
      ("help", "produce description of available options")
      ("source-file",
       value<vector<string>>(),
       "JavaScript source files")
      ("debug",
       value<bool>()->default_value(true),
       "Enable application debugging")
      ("workers",
       value<size_t>()->default_value(std::thread::hardware_concurrency()),
       "Number of thread pool workers to create");

  positional_options_description positional;

  // All arguments that are not in the options table are interpreted as source files
  positional.add("source-file", -1);

  variables_map vm;

  store(parser.options(desc).positional(positional).run(), vm);

  notify(vm);

  if (vm.count("help")) {
    stringstream ss;
    ss << desc;
    throw exceptions::CommandLineException(ss.str());
  }

  if (vm.count("source-file") == 0) {
    throw exceptions::CommandLineException("No source files were specified");
  }

  const auto sources = algorithms::mapContainer<boost::filesystem::path>(vm["source-file"].as<vector<string>>());

  return shared_ptr<const Options>(new Options(sources, vm["workers"].as<size_t>(), vm.count("debug") > 0));
}

Options::Options(const vector<boost::filesystem::path>& entries, const size_t workers, const bool debug)
  : mEntries(entries), mWorkers(workers), mDebug(debug)
{}

const std::vector<boost::filesystem::path>& Options::entries() const {
  return mEntries;
}

std::vector<boost::filesystem::path>& Options::entries() {
  return mEntries;
}

const std::size_t Options::workers() const {
  return mWorkers;
}

const bool Options::debug() const {
  return mDebug;
}
