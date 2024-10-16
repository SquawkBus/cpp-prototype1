#ifndef SQUAWKBUS_TOPICBUS_SERVER_SERVER_HPP
#define SQUAWKBUS_TOPICBUS_SERVER_SERVER_HPP

#include <string>
#include "io/poller.hpp"
#include "logging/log.hpp"

namespace squawkbus::topicbus
{
  using io::PollClient;
  using io::Poller;
  
  class EchoServer : public PollClient
  {
    void on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port) override
    {
      logging::info(std::format("on_open: {} ({}:{})", fd, host, port));
    }

    void on_close(Poller& poller, int fd) override
    {
      logging::info(std::format("on_close: {}", fd));
    }

    void on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs) override
    {
      logging::info(std::format("on_read: {}", fd));

      for (auto& buf : bufs)
      {
        std::string s {buf.begin(), buf.end()};
        logging::info(std::format("on_read: received {}", s));
        if (s == "KILLME")
        {
          logging::info(std::format("closing {}", fd));
          poller.close(fd);
        }
        else
        {
          poller.write(fd, buf);
        }
      }
    }

    void on_error(Poller& poller, int fd, std::exception error) override
    {
      logging::info(std::format("on_error: {}, {}", fd, error.what()));
    }
  };
}

#endif // SQUAWKBUS_TOPICBUS_SERVER_SERVER_HPP
