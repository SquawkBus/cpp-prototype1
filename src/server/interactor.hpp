#ifndef SQUAWKBUS_SERVER_INTERACTOR_HPP
#define SQUAWKBUS_SERVER_INTERACTOR_HPP

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "io/poller.hpp"
#include "serialization/frame_reader.hpp"

#include "messages/messages.hpp"

namespace squawkbus::server
{
  using squawkbus::io::Poller;
  using squawkbus::serialization::FrameReader;
  using squawkbus::messages::Message;

  class Hub;

  class Interactor
  {
  private:
    int fd_;
    std::string host_;
    std::string id_;
    Poller& poller_;
    Hub& hub_;
    FrameReader reader_;
    std::optional<std::string> user_;

  public:
    Interactor(int fd, Poller& poller, Hub& hub, const std::string& host, std::uint16_t port);

    int fd() const noexcept { return fd_; }
    const std::string& host() const noexcept { return host_; }
    const std::string& user() const noexcept { return *user_; }
    const std::string& id() const noexcept { return id_; }

    void receive(std::vector<char>&& buf);
    void send(std::shared_ptr<Message> message);
    std::string str() const noexcept;

  private:
    void process_message(const Message* message);
    void authenticate(const Message* message);
  };

}

#endif // SQUAWKBUS_SERVER_INTERACTOR_HPP
