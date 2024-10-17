#include "distributor.hpp"

namespace squawkbus::topicbus
{
  void Distributor::on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port)
  {
      logging::info(std::format("on_open: {} ({}:{})", fd, host, port));
  }

  void Distributor::on_close(Poller& poller, int fd)
  {
    logging::info(std::format("on_close: {}", fd));
  }

  void Distributor::on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs)
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

  void Distributor::on_error(Poller& poller, int fd, std::exception error)
  {
    logging::info(std::format("on_error: {}, {}", fd, error.what()));
  }

}