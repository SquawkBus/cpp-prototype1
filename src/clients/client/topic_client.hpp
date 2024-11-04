#ifndef SQUAWKBUS_CLIENT_TOPIC_CLIENT_HPP
#define SQUAWKBUS_CLIENT_TOPIC_CLIENT_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <variant>

#include "io/poller.hpp"
#include "io/tcp_client_socket.hpp"
#include "serialization/frame_reader.hpp"
#include "messages/messages.hpp"

namespace squawkbus::client
{
  using squawkbus::io::Poller;
  using squawkbus::io::PollClient;
  using squawkbus::io::PollHandler;
  using squawkbus::io::TcpClientSocket;
  using squawkbus::serialization::FrameReader;
  using squawkbus::messages::AuthenticationRequest;

  class TopicClient : public PollClient
  {
  private:
    std::shared_ptr<TcpClientSocket> client_socket_;
    FrameReader reader_;
    AuthenticationRequest authentication_request_;
    

  public:
    TopicClient(
      std::shared_ptr<TcpClientSocket> client_socket,
      AuthenticationRequest&& authentication_request);

    void on_startup(Poller& poller) override;
    void on_interrupt(Poller& poller) override;
    void on_open(Poller& poller, PollHandler* handler, const std::string& host, std::uint16_t port) override;
    void on_close(Poller& poller, PollHandler* handler) override;
    void on_read(Poller& poller, PollHandler* handler, std::vector<std::vector<char>>&& bufs) override;
    void on_error(Poller& poller, PollHandler* handler, std::exception error) override;

  private:
    void handle_message(Poller& poller, std::vector<char> buf);
    void handle_command(Poller& poller, std::vector<char> buf);
    void prompt() const;
  };

}

#endif // SQUAWKBUS_CLIENT_TOPIC_CLIENT_HPP
