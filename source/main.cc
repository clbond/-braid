#include <iostream>
#include <functional>

#include "io.h"
#include "options.h"
#include "thread.h"

using namespace std;
using namespace braid;

int main(const int argc, const char** argv) {
  shared_ptr<const options> options;
  try {
    boost::program_options::command_line_parser parser(argc, argv);

    options = braid::parse_command_line_arguments(parser);

    thread_pool threads;

    for (const boost::filesystem::path& path : options->entries) {
      const std::string content = stream::file::read(path);

      threads.create(content);
    }

    threads.join();
  }
  catch (const exception& e) {
    cerr << e.what() << endl;

    return 1;
  }

  return 0;
}
