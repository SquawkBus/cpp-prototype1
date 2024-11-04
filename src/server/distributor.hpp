#ifndef SQUAWKBUS_SERVER_SERVER_HPP
#define SQUAWKBUS_SERVER_SERVER_HPP

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <memory>
#include <vector>

#include "io/poller.hpp"

#include "authentication_manager.hpp"
#include "authorization_manager.hpp"
#include "interactor.hpp"
#include "hub.hpp"

namespace squawkbus::server
{
  using io::Poller;
  using io::PollClient;
  using io::PollHandler;
  
  class Distributor : public PollClient
  {
  private:
    std::map<int, std::shared_ptr<Interactor>> interactors_;
    AuthenticationManager authentication_manager_;
    Hub hub_;

  public:
    Distributor(
      AuthenticationManager&& authentication_manager,
      AuthorizationManager&& authorization_manager)
      : authentication_manager_(std::move(authentication_manager)),
        hub_(std::move(authorization_manager))
    {
    }

  private:
    // The implementation of PollClient
    void on_startup(Poller& poller) override;
    void on_interrupt(Poller& poller) override;
    void on_open(Poller& poller, PollHandler* handler, const std::string& host, std::uint16_t port) override;
    void on_close(Poller& poller, PollHandler* handler) override;
    void on_read(Poller& poller, PollHandler* handler, std::vector<std::vector<char>>&& bufs) override;
    void on_error(Poller& poller, PollHandler* handler, std::exception error) override;

  };
}

#endif // SQUAWKBUS_SERVER_SERVER_HPP
