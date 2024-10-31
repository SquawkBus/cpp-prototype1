#include "distributor.hpp"

#include <utility>

#include "logging/log.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  void Distributor::on_startup(Poller& poller)
  {
      log.debug("Starting the server.");

      hub_.on_startup();
  }

  void Distributor::on_interrupt(Poller& poller)
  {
      log.debug("Reloading the configuration.");

      hub_.on_startup();
  }

  void Distributor::on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port)
  {
      auto interactor = std::make_shared<Interactor>(fd, poller, hub_, host, port);
      log.info(std::format("Created interactor {}", interactor->str()));
      interactors_.insert({fd, interactor});
  }

  void Distributor::on_close(Poller& poller, int fd)
  {
    log.debug(std::format("Closing: fd={}", fd));

    auto i_interactor = interactors_.find(fd);
    if (i_interactor == interactors_.end())
      return;

    hub_.on_disconnected(i_interactor->second.get());
    interactors_.erase(i_interactor);
  }

  void Distributor::on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs)
  {
    log.trace(std::format("on_read: {}", fd));

    auto i_interactor = interactors_.find(fd);
    if (i_interactor == interactors_.end())
      return;

    for (auto& buf : bufs)
      i_interactor->second->receive(std::move(buf));
  }

  void Distributor::on_error(Poller& poller, int fd, std::exception error)
  {
    log.info(std::format("on_error: {}, {}", fd, error.what()));
  }

}