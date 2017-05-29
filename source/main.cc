#include <iostream>
#include <functional>
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

int main(const int argc, const char** argv) {
  boost::program_options::command_line_parser parser(argc, argv);

  exception_ptr recorded_exception;

  try {
    auto options = braid::parse_command_line_arguments(parser);

    V8::InitializeICU();
    V8::InitializeExternalStartupData(argv[0]);
    V8::InitializePlatform(v8::platform::CreateDefaultPlatform());
    V8::Initialize();

    auto dispatcher = make_shared<braid::thread::dispatcher>();

    vector<std::shared_future<v8::Local<v8::Value>>> futures;

    auto context = make_shared<vm::execution_context>();

    for (const boost::filesystem::path& path : options->entries()) {
      const auto content = stream::file::read(path);

      std::function<v8::Local<v8::Value>()> fn(
        [=]() {
          cout << "Execute: " << content << endl;
          return context->execute(content);
        });

      futures.push_back(dispatcher->dispatch<v8::Local<v8::Value>>(std::move(fn)));
    }

    dispatcher->start_workers(options->workers());

    boost::wait_for_all(futures.begin(), futures.end());

    dispatcher->stop();
    dispatcher->join();
  }
  catch (const exception& e) {
    recorded_exception = std::current_exception();
  }

  V8::Dispose();

  if (recorded_exception) {
    try {
      std::rethrow_exception(recorded_exception);
    }
    catch (const exception& e) {
      cerr << e.what() << endl;

      return 1; // failure due to uncaught exception
    }
  }

  return 0;
}
