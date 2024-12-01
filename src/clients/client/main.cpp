#include <signal.h>

#include <csignal>
#include <cstdio>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <span>
#include <utility>
#include <variant>

#include "io/file.hpp"
#include "io/poller.hpp"
#include "io/file_poll_handler.hpp"
#include "io/tcp_client_socket.hpp"
#include "io/tcp_socket_poll_handler.hpp"
#include "io/ssl_ctx.hpp"
#include "logging/log.hpp"
#include "utils/utils.hpp"

#include "options.hpp"
#include "squawkbus_client.hpp"

using namespace squawkbus::io;
namespace logging = squawkbus::logging;
using squawkbus::client::Options;
using squawkbus::client::SquawkbusClient;

namespace
{
  volatile std::sig_atomic_t last_signal = 0;
}

int main(int argc, char** argv)
{
  auto options = Options::parse(argc, argv);

  try
  {
    auto ssl_ctx = options.make_ssl_context();

    print_line(std::format(
      "connecting to host {} on port {}{}.",
      options.host,
      options.port,
      (options.tls ? " using tls" : "")));

    auto client_socket = std::make_shared<TcpClientSocket>();
    client_socket->connect(options.host, options.port);
    client_socket->blocking(false);

    auto authentication_request = options.make_authentication_request();

    auto client = std::make_shared<SquawkbusClient>(
      client_socket,
      std::move(authentication_request));
      
    auto poller = Poller(client);

    auto handler = ssl_ctx
      ? std::make_unique<TcpSocketPollHandler>(client_socket, *ssl_ctx, options.host, 8096, 8096)
      : std::make_unique<TcpSocketPollHandler>(client_socket, 8096, 8096);

    poller.add_handler(
      std::move(handler),
      options.host,
      options.port);

    auto console_input = std::make_shared<File>(STDIN_FILENO, O_RDONLY);
    console_input->blocking(false);
    poller.add_handler(std::make_unique<FilePollHandler>(console_input, 1024, 1024), "localhost", 0);

    poller.event_loop(last_signal);
  }
  catch(const std::exception& error)
  {
    logging::error(std::format("Server failed: {}", error.what()));
  }
  catch (...)
  {
    logging::error("unknown error");
  }
 
  return 0;
}