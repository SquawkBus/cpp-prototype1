#include <cstdio>
#include <format>
#include <iomanip>
#include <memory>
#include <span>
#include <string>
#include <utility>
#include <vector>


#include "io/poller.hpp"
#include "io/tcp_client_socket.hpp"
#include "logging/log.hpp"
#include "utils/utils.hpp"
#include "messages/messages.hpp"

#include "squawkbus_client.hpp"
#include "utils.hpp"

namespace squawkbus::client
{
  using squawkbus::io::Poller;
  using squawkbus::io::PollClient;
  using squawkbus::io::TcpClientSocket;
  using squawkbus::messages::Message;
  using squawkbus::messages::AuthenticationRequest;
  using squawkbus::messages::NotificationRequest;
  using squawkbus::messages::SubscriptionRequest;
  using squawkbus::messages::MulticastData;
  using squawkbus::messages::DataPacket;

  SquawkbusClient::SquawkbusClient(
    std::shared_ptr<TcpClientSocket> client_socket,
    AuthenticationRequest&& authenticate)
    : client_socket_(client_socket),
      authentication_request_(std::move(authenticate))
  {
  }

  void SquawkbusClient::on_startup(Poller& poller)
  {
    logging::info("on_startup");

    auto frame = authentication_request_.serialize();
    auto buf = std::vector<char>(frame);
    poller.write(client_socket_->fd(), buf);

    prompt();
  }

  void SquawkbusClient::on_interrupt(Poller& poller)
  {
    logging::info("on_interrupt");
  }

  void SquawkbusClient::on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port)
  {
    logging::info(std::format("on_open: {} ({}:{})", fd, host, port));
  }

  void SquawkbusClient::on_close(Poller& poller, int fd)
  {
    logging::info(std::format("on_close: {}", fd));
    exit(0);
  }

  void SquawkbusClient::on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs)
  {
    logging::info(std::format("on_read: {}", fd));

    for (auto& buf : bufs)
    {
      if (fd == STDIN_FILENO)
      {
        handle_command(poller, buf);
      }
      else if (fd == client_socket_->fd())
      {
        handle_message(poller, buf);
      }
    }
  }

  void SquawkbusClient::on_error(Poller& poller, int fd, std::exception error)
  {
    logging::info(std::format("on_error: {} - {}", fd, error.what()));
  }

  void SquawkbusClient::handle_command(Poller& poller, std::vector<char> buf)
  {
    auto line = std::string(buf.begin(), buf.end());
    logging::info(std::format("on_read: received {}", line));

    auto ss = std::stringstream(line);

    std::string command;
    ss >> command;

    if (command == "CLOSE")
    {
      poller.close(client_socket_->fd());
    }
    else if (command == "SUBSCRIBE")
    {
      std::string topic;
      ss >> topic;
      auto message = SubscriptionRequest(topic, true);
      auto frame = message.serialize();
      auto response = std::vector<char>(frame);
      poller.write(client_socket_->fd(), response);
    }
    else if (command == "UNSUBSCRIBE")
    {
      std::string topic;
      ss >> topic;
      auto message = SubscriptionRequest(topic, false);
      auto frame = message.serialize();
      auto response = std::vector<char>(frame);
      poller.write(client_socket_->fd(), response);
    }
    else if (command == "LISTEN")
    {
      std::string topic;
      ss >> topic;
      auto message = NotificationRequest(topic, true);
      auto frame = message.serialize();
      auto response = std::vector<char>(frame);
      poller.write(client_socket_->fd(), response);
    }
    else if (command == "UNLISTEN")
    {
      std::string topic;
      ss >> topic;
      auto message = NotificationRequest(topic, false);
      auto frame = message.serialize();
      auto response = std::vector<char>(frame);
      poller.write(client_socket_->fd(), response);
    }
    else if (command == "PUBLISH")
    {
      std::string topic, content, content_type;
      int entitlement = 0;
      ss
        >> std::quoted(topic)
        >> std::quoted(content)
        >> std::quoted(content_type)
        >> entitlement;
      if (content_type.empty())
        content_type = "text/plain";
      auto data_packet = DataPacket(
        entitlement,
        content_type,
        std::vector<char>(content.begin(), content.end()));
      auto message = MulticastData(topic, { data_packet });
      auto frame = message.serialize();
      auto response = std::vector<char>(frame);
      poller.write(client_socket_->fd(), response);
    }
    else
    {
      auto msg = std::format("unknown command: {}", command);
      std::puts(msg.c_str());
    }
  }

  void SquawkbusClient::handle_message(Poller& poller, std::vector<char> buf)
  {
    reader_.write(buf);

    while (reader_.has_frame())
    {
      auto frame = reader_.read();
      auto message = Message::deserialize(frame);
      auto text = std::format("on_message: {}", message->str());
      std::puts(text.c_str());
    }

    prompt();
  }

  void SquawkbusClient::prompt() const
  {
    std::stringstream ss;
    ss
      << "Usage: <PUBLISH | SUBSCRIBE | UNSUBSCRIBE | LISTEN | UNLISTEN> <options...>\n"
      << "PUBLISH <topic> <content> [<content-type> [<entitlement>]]\n"
      << "SUBSCRIBE <topic>\n"
      << "UNSUBSCRIBE <topic>\n"
      << "LISTEN <regex>\n"
      << "UNLISTEN <regex>\n"
      ;
    std::fputs(ss.str().c_str(), stdout);
  }

}
