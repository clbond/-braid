#pragma once

#include <boost/noncopyable.hpp>

#include <boost/asio/io_service.hpp>

namespace braid {
  class thread : private boost::noncopyable {
    public:
      const boost::asio::io_service& service() const {
        return service_;
      }

      boost::asio::io_service& service() {
        return service_;
      }

    private:
      boost::asio::io_service service_;
  };
}