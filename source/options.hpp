#pragma once

#include <memory>
#include <vector>
#include <string>

#include <boost/filesystem/path.hpp>

#include <boost/program_options.hpp>

namespace braid {
  class Options {
    public:
      static std::shared_ptr<const Options> parseCommandLine(boost::program_options::command_line_parser& parser);

      const std::vector<boost::filesystem::path>& entries() const;

      std::vector<boost::filesystem::path>& entries();

      const std::size_t workers() const;

      const bool debug() const;

    protected:
      Options(const std::vector<boost::filesystem::path>& entries, const std::size_t workers, const bool debug);

    private:
      std::vector<boost::filesystem::path> mEntries;
      std::size_t mWorkers;
      bool mDebug;
  };
}