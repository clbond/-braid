#include <iostream>
#include <functional>

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

    shared_ptr<braid::thread::pool> threads(new braid::thread::pool());

    vector<shared_ptr<braid::script::executable_code<>>> code;

    script::executable_code_factory factory;

    for (const boost::filesystem::path& path : options->entries) {
      const string content = stream::file::read(path);

      auto c = factory.from_string(content);

      c->operator()(threads);

      code.push_back(c);
    }

    threads->start_workers(options->workers);

    for (auto& executable: code) {
      executable->get_future().wait();
    }

    threads->stop();

    threads->join();
  }
  catch (const exception& e) {
    cerr << "Failure: " << e.what() << endl;

    return 1;
  }

  return 0;
}
