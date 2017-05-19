#include <iostream>

#include <v8.h>

#include "options.h"

#include "thread/pool.h"
#include "io/stream.h"
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

    thread_pool threads;

    for (const boost::filesystem::path& path : options->entries) {
      const std::string content = stream::file::read(path);

      auto thread = threads.create(content);

      thread->service().post([]() {
        cout << "HELLO!!!" << endl;
      });
    }

    sleep(1);

    threads.join();
  }
  catch (const exception& e) {
    cerr << e.what() << endl;

    return 1;
  }

  return 0;
}
