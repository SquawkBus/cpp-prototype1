#include <cstdio>
#include <format>
#include <memory>
#include <span>
#include <utility>
#include <vector>


#include "io/poller.hpp"
#include "io/tcp_client_socket.hpp"
#include "logging/log.hpp"
#include "utils/utils.hpp"
#include "messages/messages.hpp"

#include "topic_client.hpp"
#include "utils.hpp"

namespace squawkbus::client
{
  using squawkbus::io::Poller;
  using squawkbus::io::PollClient;
  using squawkbus::io::TcpClientSocket;
  using squawkbus::messages::Message;
  using squawkbus::messages::Authenticate;
  using squawkbus::messages::SubscriptionRequest;
  using squawkbus::messages::MulticastData;
  using squawkbus::messages::DataPacket;

  TopicClient::TopicClient(std::shared_ptr<TcpClientSocket> client_socket)
    : client_socket_(client_socket)
  {
  }

  void TopicClient::on_startup(Poller& poller)
  {
    logging::info("on_startup");

    auto user = std::string("johndoe");
    auto data = std::vector(user.begin(), user.end());
    auto msg = Authenticate("PLAIN", data);
    auto frame = msg.serialize();
    auto buf = std::vector<char>(frame);
    logging::info(std::format("authenticating as {}", user));
    poller.write(client_socket_->fd(), buf);
  }

  void TopicClient::on_open(Poller& poller, int fd, const std::string& host, std::uint16_t port)
  {
    logging::info(std::format("on_open: {} ({}:{})", fd, host, port));
  }

  void TopicClient::on_close(Poller& poller, int fd)
  {
    logging::info(std::format("on_close: {}", fd));
  }

  void TopicClient::on_read(Poller& poller, int fd, std::vector<std::vector<char>>&& bufs)
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

  void TopicClient::on_error(Poller& poller, int fd, std::exception error)
  {
    logging::info(std::format("on_error: {} - {}", fd, error.what()));
  }

  void TopicClient::handle_command(Poller& poller, std::vector<char> buf)
  {
    auto line = std::string(buf.begin(), buf.end());
    logging::info(std::format("on_read: received {}", line));

    auto words = split(line, " ");
    for (auto& word : words)
      trim(word);

    if (words.size() < 1)
      return;

    auto command = words[0];

    if (command == "CLOSE")
    {
      poller.close(client_socket_->fd());
    }
    else if (words.size() == 2 && command == "SUBSCRIBE")
    {
      auto topic_pattern = words[1];
      auto message = SubscriptionRequest(topic_pattern, true);
      auto frame = message.serialize();
      auto response = std::vector<char>(frame);
      poller.write(client_socket_->fd(), response);
    }
    else if (words.size() == 2 && command == "UNSUBSCRIBE")
    {
      auto topic_pattern = words[1];
      auto message = SubscriptionRequest(topic_pattern, false);
      auto frame = message.serialize();
      auto response = std::vector<char>(frame);
      poller.write(client_socket_->fd(), response);
    }
    else if (words.size() == 3 && command == "PUBLISH")
    {
      auto topic = words[1];
      auto content = words[2];
      auto data_packet = DataPacket(
        0,
        "text/plain",
        std::vector<char>(content.begin(), content.end()));
      auto message = MulticastData(topic, { data_packet });
      auto frame = message.serialize();
      auto response = std::vector<char>(frame);
      poller.write(client_socket_->fd(), response);
    }
    else
    {
      auto msg = std::format("unknown command: {}", command);
      auto response = std::vector<char>(msg.begin(), msg.end());
      poller.write(STDOUT_FILENO, response);
    }
  }

  void TopicClient::handle_message(Poller& poller, std::vector<char> buf)
  {
    reader_.write(buf);

    while (reader_.has_frame())
    {
      auto frame = reader_.read();
      auto message = Message::deserialize(frame);
      auto text = std::format("on_message: {}", message->str());
      std::puts(text.c_str());
    }
  }

}
