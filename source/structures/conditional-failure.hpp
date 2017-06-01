#pragma once

#include <system_error>
#include <functional>
#include <stdexcept>
#include <string>

namespace braid::structures {

class ConditionalFailure {
  public:
    ConditionalFailure(const std::string& message, const std::function<bool()> failed)
      : mMessage(message), mFailed(failed)
    {
      maybeThrow();
    }

    virtual ~ConditionalFailure() {
      maybeThrow();
    }

    virtual void maybeThrow() {
      if (mFailed()) {
        throw std::system_error(errno, std::system_category(), mMessage.c_str());
      }
    }

    const std::string mMessage;

    const std::function<bool()> mFailed;
};

} // namespace braid::algorithms