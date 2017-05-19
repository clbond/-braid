#include <iostream>

#include <v8.h>

#include "execution-context.h"
#include "io.h"
#include "options.h"
#include "platform.h"

using namespace v8;
using namespace std;
using namespace braid;

int main(const int argc, const char** argv) {
  auto options = Options::parse(argc, argv);

  if (options.get() == nullptr) {
    return 1;
  }

  V8::InitializeICU(argv[0]);
  V8::InitializeExternalStartupData(argv[0]);

  braid::Platform platform;
  try {
    auto context = platform.createExecutionContext();

    for (const string& file : options->entries) {
      cout << "load file: "<< file<< endl;
      context->run(File::read(file));
    }

    context->join();
  }
  catch (const exception& ex) {
    cerr << "Failed to execute application: " << ex.what() << endl;
    return 1;
  }
  return 0;
}
