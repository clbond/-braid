#pragma once

#include <memory>
#include <vector>
#include <string>

namespace braid {
  struct Options {
    static std::shared_ptr<const Options> parse(const int argc, const char** argv);

    bool debug;

    std::vector<std::string> entries;
  };
}