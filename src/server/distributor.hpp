#ifndef SQUAWKBUS_SERVER_SERVER_HPP
#define SQUAWKBUS_SERVER_SERVER_HPP

#include <map>
#include <string>
#include <memory>

#include "io/poller.hpp"

#include "interactor.hpp"
#include "hub.hpp"

namespace squawkbus::server
{
  using io::PollClient;
  using io::Poller;
  
  class Distributor : public PollClient
  {
  private:
    std::map<int, std::shared_ptr<Interactor>> interactors_;
    Hub hub_;

  private:
    // The implementation of PollClient
    void on_startup(Poller& poller) override;
    void on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port) override;
    void on_close(Poller& poller, int fd) override;
    void on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs) override;
    void on_error(Poller& poller, int fd, std::exception error) override;

  };
}

#endif // SQUAWKBUS_SERVER_SERVER_HPP
