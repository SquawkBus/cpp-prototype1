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
  using squawkbus::messages::Authenticate;

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
      auto s = std::string(buf.begin(), buf.end());

      logging::info(std::format("on_read: received {}", s));
      if (fd == STDIN_FILENO)
      {
        auto line = split(s, " ");
        for (auto& word : line)
          trim(word);

        if (s == "CLOSE")
        {
          poller.close(client_socket_->fd());
        }
        else if (s == "SUBSCRIBE")
        {
          // Subscribe
        }
        else
        {
          poller.write(client_socket_->fd(), buf);
        }
      }
      else if (fd == client_socket_->fd())
      {
        poller.write(STDOUT_FILENO, buf);
      }
    }
  }

  void TopicClient::on_error(Poller& poller, int fd, std::exception error)
  {
    logging::info(std::format("on_error: {} - {}", fd, error.what()));
  }

}
