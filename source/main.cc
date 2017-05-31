#include <iostream>
#include <functional>
#include <future>
#include <utility>

#include <boost/bind.hpp>
#include <boost/thread/futures/wait_for_all.hpp>

#include <v8.h>
#include <v8-platform.h>

#include <libplatform/libplatform.h>

#include "io.hpp"
#include "options.hpp"
#include "thread.hpp"
#include "vm.hpp"

using namespace std;
using namespace braid;
using namespace v8;

int main(int argc, char** argv) {
  boost::program_options::command_line_parser parser(argc, argv);

  exception_ptr recorded_exception;

  try {
    auto options = Options::parseCommandLine(parser);

    V8::InitializeICU();
    V8::SetFlagsFromCommandLine(&argc, argv, true);
    V8::InitializeExternalStartupData(argv[0]);
    V8::InitializePlatform(v8::platform::CreateDefaultPlatform());
    V8::Initialize();

    auto dispatcher = make_shared<braid::thread::Dispatcher>();

    vector<shared_future<void>> futures;

    vector<shared_ptr<promise<void>>> promises;

    for (const boost::filesystem::path& path : options->entries()) {
      auto promise = make_shared<std::promise<void>>();

      promises.push_back(promise);

      futures.push_back(dispatcher->dispatch(promise, std::bind(vm::executeInIsolation, stream::read(path))));
    }

    dispatcher->start_workers(options->workers());

    braid::thread::waitForFutures(futures.begin(), futures.end());

    dispatcher->stop();
    dispatcher->join();
  }
  catch (const exception& e) {
    recorded_exception = current_exception();
  }

  V8::Dispose();

  if (recorded_exception) {
    try {
      rethrow_exception(recorded_exception);
    }
    catch (const exception& e) {
      cerr << e.what() << endl;

      return 1; // failure due to uncaught exception
    }
  }

  return 0;
}
