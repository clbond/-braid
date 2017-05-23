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

    shared_ptr<braid::thread::pool> threads(new braid::thread::pool());

    threads->start_workers(options->workers);

    vector<shared_ptr<promise<void>>> promises;

    script::execution_factory factory(threads);

    for (const boost::filesystem::path& path : options->entries) {
      const string content = stream::file::read(path);

      shared_ptr<script::execution<void>> execution = factory.from_string(content);

      shared_ptr<promise<void>> promise(new std::promise<void>());

      promises.push_back(promise);

      execution->run(*promise.get());
    }

    for (auto&& promise : promises) {
      promise->get_future().wait();
    }

    threads->join();
  }
  catch (const exception& e) {
    cerr << e.what() << endl;

    return 1;
  }

  return 0;
}
