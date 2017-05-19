#include <iostream>

#include <v8.h>

#include "options.h"

#include "v8/platform.h"

using namespace v8;
using namespace std;
using namespace braid;

int main(const int argc, const char** argv) {
  shared_ptr<const options> options;
  try {
    boost::program_options::command_line_parser parser(argc, argv);

    options = braid::parse_cli(parser);

    V8::InitializeICU(argv[0]);
    V8::InitializeExternalStartupData(argv[0]);

    braid::v8::platform platform;

    for (const boost::filesystem::path& path : options->entries) {
      cout << "Source file: " << path << endl;
    }
  }
  catch (const exception& e) {
    cerr << e.what() << endl;
    return 1;
  }

  return 0;
}
