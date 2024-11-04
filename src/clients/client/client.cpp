#include "client.hpp"

#include <stdexcept>

#include "logging/log.hpp"

namespace squawkbus::client
{
  using squawkbus::io::Poller;
  using squawkbus::io::PollClient;
  using squawkbus::io::PollHandler;
  using squawkbus::io::TcpClientSocket;
  using squawkbus::messages::DataPacket;
  using squawkbus::messages::Message;
  using squawkbus::messages::MessageType;
  using squawkbus::messages::AuthenticationRequest;
  using squawkbus::messages::AuthenticationResponse;
  using squawkbus::messages::NotificationRequest;
  using squawkbus::messages::SubscriptionRequest;
  using squawkbus::messages::MulticastData;
  using squawkbus::messages::UnicastData;
  using squawkbus::messages::DataPacket;

  Client::Client(
    std::shared_ptr<TcpClientSocket> client_socket,
    AuthenticationRequest&& authenticate)
    : client_socket_(client_socket),
      authentication_request_(std::move(authenticate))
  {
  }

  void Client::on_startup(Poller& poller)
  {
    poller_ = poller;
    logging::info("Sending authentication request");
    auto frame = authentication_request_.serialize();
    auto buf = std::vector<char>(frame);
    poller.write(client_socket_->fd(), buf);
  }

  void Client::on_interrupt(Poller& poller)
  {
  }

  void Client::on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port)
  {
  }

  void Client::on_close(Poller& poller, int fd)
  {
    // TODO: manage connection state.
  }

  void Client::on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs)
  {
    if (fd != client_socket_->fd())
    {
      logging::error(std::format("invalid fd {}", fd));
      return;
    }

    for (auto& buf : bufs)
    {
      reader_.write(buf);
    }

    while (reader_.has_frame())
    {
      auto frame = reader_.read();
      auto message = Message::deserialize(frame);
      switch (message->message_type)
      {
        case MessageType::AuthenticationResponse:
          handle_authentication_response(*std::static_pointer_cast<AuthenticationResponse>(message));
          break;

        case MessageType::ForwardedMulticastData:
          handle_forwarded_multicast_data(*std::static_pointer_cast<ForwardedMulticastData>(message));
          break;

        case MessageType::ForwardedUnicastData:
          handle_forwarded_unicast_data(*std::static_pointer_cast<ForwardedUnicastData>(message));
          break;

        case MessageType::ForwardedSubscriptionRequest:
          handle_forwarded_subscription_request(*std::static_pointer_cast<ForwardedSubscriptionRequest>(message));
          break;

        case MessageType::AuthenticationRequest:
        case MessageType::MulticastData:
        case MessageType::UnicastData:
        case MessageType::NotificationRequest:
        case MessageType::SubscriptionRequest:
        default:
          throw std::runtime_error("invalid message type");
      }
      auto text = std::format("on_message: {}", message->str());
      std::puts(text.c_str());
    }
  }

  void Client::handle_authentication_response(const AuthenticationResponse& message)
  {
    // TODO: maintain connection state.
  }

  void Client::handle_forwarded_multicast_data(const ForwardedMulticastData& message)
  {
  }

  void Client::handle_forwarded_unicast_data(const ForwardedUnicastData& message)
  {
  }

  void Client::handle_forwarded_subscription_request(const ForwardedSubscriptionRequest& message)
  {
  }

  void Client::publish(const std::string& topic, const std::vector<DataPacket>& data_packets)
  {
      auto message = std::make_unique<MulticastData>(topic, data_packets);
      write(message.get());
  }

  void Client::send(const std::string& client_id, const std::string& topic, const std::vector<DataPacket>& data_packets)
  {
      auto message = std::make_unique<UnicastData>(client_id, topic, data_packets);
      write(message.get());
  }

  void Client::subscribe(const std::string& topic)
  {
      auto message = std::make_unique<SubscriptionRequest>(topic, true);
      write(message.get());
  }

  void Client::unsubscribe(const std::string& topic)
  {
      auto message = std::make_unique<SubscriptionRequest>(topic, false);
      write(message.get());
  }

  void Client::listen(const std::string& topic_pattern)
  {
      auto message = std::make_unique<NotificationRequest>(topic_pattern, true);
      write(message.get());
  }

  void Client::unlisten(const std::string& topic_pattern)
  {
      auto message = std::make_unique<NotificationRequest>(topic_pattern, false);
      write(message.get());
  }

  void Client::write(const Message* message)
  {
    auto frame = message->serialize();
    auto response = std::vector<char>(frame);
    if (poller_)
      poller_->write(client_socket_->fd(), response);
  }
}