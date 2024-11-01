#include "interactor.hpp"

#include <format>
#include <stdexcept>
#include <string>

#include "logging/log.hpp"

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "authentication_manager.hpp"
#include "hub.hpp"
#include "uuid.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  using squawkbus::io::Poller;
  using squawkbus::serialization::FrameBuffer;
  using squawkbus::messages::Message;
  using squawkbus::messages::MessageType;
  using squawkbus::messages::Authenticate;

  Interactor::Interactor(
    int fd,
    Poller& poller,
    AuthenticationManager& authentication_manager,
    Hub& hub,
    const std::string& host,
    std::uint16_t port)
    : fd_(fd),
      host_(host),
      id_(uuid::generate().str()),
      poller_(poller),
      authentication_manager_(authentication_manager),
      hub_(hub)
  {
  }

  void Interactor::send(std::shared_ptr<Message> message)
  {
    auto frame = message->serialize();
    auto buf = std::vector<char>(frame);
    poller_.write(fd_, buf);
  }

  void Interactor::receive(std::vector<char>&& buf)
  {
    reader_.write(buf);

    while (reader_.has_frame())
    {
      auto frame = reader_.read();
      auto message = Message::deserialize(frame);
      process_message(message.get());
    }
  }

  void Interactor::process_message(Message* message)
  {
    if (user_ == std::nullopt)
      authenticate(message);
    else
      hub_.on_message(this, message);
  }

  void Interactor::authenticate(Message* message)
  {
    if (message->message_type() != MessageType::Authenticate)
      throw std::runtime_error("expected authenticate message");

    auto authenticate_message = *dynamic_cast<Authenticate*>(message);
    if (authenticate_message.method() == "NONE")
    {
      user_ = authenticate_message.data().empty()
        ? std::string("nobody")
        : std::string(
            authenticate_message.data().begin(),
            authenticate_message.data().end());
      log.info(std::format("Authenticated {}.", str()));
      hub_.on_connected(this);
      return;
    }
    else if (authenticate_message.method() == "HTPASSWD")
    {
      auto frame = FrameBuffer::from(std::move(authenticate_message.data()));
      std::string username, password;
      frame >> username >> password;
      if (!authentication_manager_.authenticate(username, password))
        throw std::runtime_error("Failed to authenticate");
      log.info(std::format("Authenticated {}.", str()));
      hub_.on_connected(this);
      return;
    }

    throw std::runtime_error("unknown authentication method");
  }

  std::string Interactor::str() const noexcept
  {
    auto user = user_.has_value() ? user_.value() : "unauthenticated";
    return std::format("<interactor [{}/{}] ({})>", id_, fd_, user);
  }
}