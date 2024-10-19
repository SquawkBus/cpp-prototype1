#ifndef SQUAWKBUS_TOPICBUS_SERVER_INTERACTOR_HPP
#define SQUAWKBUS_TOPICBUS_SERVER_INTERACTOR_HPP

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "io/poller.hpp"
#include "serialization/frame_reader.hpp"

#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus
{
  using squawkbus::io::Poller;
  using squawkbus::serialization::FrameReader;
  using squawkbus::topicbus::messages::Message;

  class Interactor
  {
  private:
    int fd_;
    std::string host_;
    std::string id_;
    Poller& poller_;
    FrameReader reader_;
    std::optional<std::string> user_;

  public:
    Interactor(int fd, Poller& poller, const std::string& host, std::uint16_t port);

    int fd() const noexcept { return fd_; }
    const std::string& host() const noexcept { return host_; }

    void receive(std::vector<char>&& buf);
    void send(std::shared_ptr<Message> message);

  private:
    void process_message(std::shared_ptr<Message> message);
    void authenticate(std::shared_ptr<Message> message);
  };

}

#endif // SQUAWKBUS_TOPICBUS_SERVER_INTERACTOR_HPP
