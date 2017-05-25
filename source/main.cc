#include <iostream>
#include <functional>
#include <utility>

#include <boost/thread/futures/wait_for_all.hpp>

#include "io.h"
#include "options.h"
#include "thread.h"
#include "vm.h"

using namespace std;
using namespace braid;

int main(const int argc, const char** argv) {
  boost::program_options::command_line_parser parser(argc, argv);

  try {
    auto options = braid::parse_command_line_arguments(parser);

    shared_ptr<braid::thread::dispatcher> dispatcher(new braid::thread::dispatcher());

    vector<std::shared_future<void>> futures;

    shared_ptr<vm::machine> virtual_machine(new vm::machine());

    for (const boost::filesystem::path& path : options->entries()) {
      function<void()> fn = [=]() {
        const auto content = stream::file::read(path);

        auto compilation_result = virtual_machine->compile(content);

        auto execution_result = virtual_machine->execute(compilation_result);

        std::cout << "Script: " << content << std::endl;
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
