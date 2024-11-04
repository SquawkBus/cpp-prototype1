#ifndef SQUAWKBUS_CLIENT_CLIENT_HPP
#define SQUAWKBUS_CLIENT_CLIENT_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "io/poller.hpp"
#include "io/tcp_client_socket.hpp"
#include "messages/messages.hpp"
#include "serialization/frame_reader.hpp"

namespace squawkbus::client
{
  using squawkbus::io::Poller;
  using squawkbus::io::PollClient;
  using squawkbus::io::PollHandler;
  using squawkbus::io::TcpClientSocket;
  using squawkbus::serialization::FrameReader;
  using squawkbus::messages::Message;
  using squawkbus::messages::DataPacket;
  using squawkbus::messages::AuthenticationRequest;
  using squawkbus::messages::AuthenticationResponse;
  using squawkbus::messages::ForwardedMulticastData;
  using squawkbus::messages::ForwardedUnicastData;
  using squawkbus::messages::ForwardedSubscriptionRequest;

  class Client : public PollClient
  {
    std::shared_ptr<TcpClientSocket> client_socket_;
    FrameReader reader_;
    AuthenticationRequest authentication_request_;
    std::optional<Poller&> poller_;

  public:
    Client(
      std::shared_ptr<TcpClientSocket> client_socket,
      AuthenticationRequest&& authentication_request);

    void on_startup(Poller& poller) override;
    void on_interrupt(Poller& poller) override;
    void on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port) override;
    void on_close(Poller& poller, int fd) override;
    void on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs) override;
    void on_error(Poller& poller, int fd, std::exception error) override;

    void publish(const std::string& topic, const std::vector<DataPacket>& data_packets);
    void send(const std::string& client_id, const std::string& topic, const std::vector<DataPacket>& data_packets);

    void subscribe(const std::string& topic);
    void unsubscribe(const std::string& topic);

    void listen(const std::string& topic_pattern);
    void unlisten(const std::string& topic_pattern);
    
  private:
    void handle_message(Poller& poller, std::vector<char> buf);

    void handle_authentication_response(const AuthenticationResponse& message);
    void handle_forwarded_multicast_data(const ForwardedMulticastData& message);
    void handle_forwarded_unicast_data(const ForwardedUnicastData& message);
    void handle_forwarded_subscription_request(const ForwardedSubscriptionRequest& message);

    void write(const Message* message);
  };
}

#endif // SQUAWKBUS_CLIENT_CLIENT_HPP
