#include <iostream>
#include <functional>
#include <utility>

#include <boost/thread/futures/wait_for_all.hpp>

#include "io.h"
#include "options.h"
#include "script.h"
#include "thread.h"

using namespace std;
using namespace braid;

int main(const int argc, const char** argv) {
  boost::program_options::command_line_parser parser(argc, argv);

  try {
    auto options = braid::parse_command_line_arguments(parser);

    shared_ptr<braid::thread::dispatcher> dispatcher(new braid::thread::dispatcher());

    vector<std::shared_future<void>> futures;

    for (const boost::filesystem::path& path : options->entries()) {
      const string content = stream::file::read(path);

      std::function<void()> fn = [=]() {
        std::cout << "Parse: " << content << std::endl;
      };

      std::shared_future<void> future = dispatcher->dispatch(fn);

      futures.push_back(future);
    }

    dispatcher->start_workers(options->workers());

    boost::wait_for_all(futures.begin(), futures.end());

    dispatcher->stop();
    dispatcher->join();
  }
  catch (const exception& e) {
    cerr << "Failure: " << e.what() << endl;

    return 1;
  }

  return 0;
}
