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

      authentication_manager_.load();
      hub_.on_startup();
  }

  void Distributor::on_interrupt(Poller& poller)
  {
      log.debug("Reloading the configuration.");

      authentication_manager_.load();
      hub_.on_startup();
  }

  void Distributor::on_open(Poller& poller, PollHandler* handler, const std::string& host, std::uint16_t port)
  {
      auto interactor = std::make_shared<Interactor>(handler->fd(), poller, authentication_manager_, hub_, host, port);
      log.info(std::format("Created interactor {}", interactor->str()));
      interactors_.insert({handler->fd(), interactor});
  }

  void Distributor::on_close(Poller& poller, PollHandler* handler)
  {
    log.debug(std::format("Closing: fd={}", handler->fd()));

    auto i_interactor = interactors_.find(handler->fd());
    if (i_interactor == interactors_.end())
      return;

    hub_.on_disconnected(i_interactor->second.get());
    interactors_.erase(i_interactor);
  }

  void Distributor::on_read(Poller& poller, PollHandler* handler, std::vector<std::vector<char>>&& bufs)
  {
    log.trace(std::format("on_read: {}", handler->fd()));

    auto i_interactor = interactors_.find(handler->fd());
    if (i_interactor == interactors_.end())
      return;

    for (auto& buf : bufs)
      i_interactor->second->receive(std::move(buf));
  }

  void Distributor::on_error(Poller& poller, PollHandler* handler, std::exception error)
  {
    log.info(std::format("on_error: {}, {}", handler->fd(), error.what()));
  }

}