#include "distributor.hpp"

namespace squawkbus::topicbus
{
  void Distributor::on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port)
  {
      logging::info(std::format("on_open: {} ({}:{})", fd, host, port));

      interactors_.insert(std::make_pair(
        fd,
        Interactor(fd, poller, host, port)));
  }

  void Distributor::on_close(Poller& poller, int fd)
  {
    logging::info(std::format("on_close: {}", fd));

    auto i_interactor = interactors_.find(fd);
    if (i_interactor == interactors_.end())
      return;

    interactors_.erase(i_interactor);
  }

  void Distributor::on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs)
  {
    logging::info(std::format("on_read: {}", fd));

    auto i_interactor = interactors_.find(fd);
    if (i_interactor == interactors_.end())
      return;

    for (auto& buf : bufs)
      i_interactor->second.receive(std::move(buf));
  }

  void Distributor::on_error(Poller& poller, int fd, std::exception error)
  {
    logging::info(std::format("on_error: {}, {}", fd, error.what()));
  }

}