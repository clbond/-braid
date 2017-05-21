#include <iostream>
#include <functional>

#include "io.h"
#include "options.h"
#include "script.h"
#include "thread.h"

using namespace std;
using namespace braid;

int main(const int argc, const char** argv) {
  shared_ptr<const options> options;
  try {
    boost::program_options::command_line_parser parser(argc, argv);

    options = braid::parse_command_line_arguments(parser);

    shared_ptr<thread_pool> threads(new thread_pool());

    threads->start_workers(options->workers);

    vector<future<void>> futures;

    for (const boost::filesystem::path& path : options->entries) {
      const string content = stream::file::read(path);

      shared_ptr<const script::runnable> script = script::parse(content);

      futures.push_back(
        threads->task(
          std::packaged_task<void()>([&]() {
            script->run(threads);
          })));
    }

    for_each(futures.begin(), futures.end(), [](future<void>& f) {
      f.wait();
    });

    threads->stop();

    threads->join();
  }
  catch (const exception& e) {
    cerr << e.what() << endl;

    return 1;
  }

  return 0;
}
