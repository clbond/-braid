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

class runner {
  public:
    runner(shared_ptr<vm::execution_context> c, const string code)
      : context(c), content(code)
    {}

    std::function<v8::Local<v8::Value>()> fn() const {
      return [=]() {
        cout << "SON I BE EXECUTING: " << content << " ON " << context.get() << endl;

        auto result = context->execute(content);

        cout << "SON TEH EXECUTE IS TEH DONE" << endl;
        //delete this;

        v8::String::Utf8Value utf(result);
        cout << "SON TEH RESULT" << *utf << endl;

        return result;
      };
    }

    v8::Local<v8::Value> operator()() {
      return context->execute(content);
    }

  private:
    shared_ptr<vm::execution_context> context;

    const string content;
};

int main(int argc, char** argv) {
  boost::program_options::command_line_parser parser(argc, argv);

  exception_ptr recorded_exception;

  try {
    auto options = braid::parse_command_line_arguments(parser);

    V8::InitializeICU();
    V8::SetFlagsFromCommandLine(&argc, argv, true);
    V8::InitializeExternalStartupData(argv[0]);
    V8::InitializePlatform(v8::platform::CreateDefaultPlatform());
    V8::Initialize();

    auto dispatcher = make_shared<braid::thread::dispatcher>();

    vector<std::shared_future<v8::Local<v8::Value>>> futures;

    for (const boost::filesystem::path& path : options->entries()) {
      auto context = make_shared<vm::execution_context>();

      runner* task = new runner(context, stream::file::read(path));

      futures.push_back(dispatcher->dispatch(task->fn()));
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
