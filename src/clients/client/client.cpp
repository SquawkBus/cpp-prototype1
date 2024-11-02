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
#include "io/tcp_stream.hpp"
#include "io/ssl_ctx.hpp"
#include "logging/log.hpp"
#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"
#include "utils/match.hpp"
#include "utils/utils.hpp"

#include "popl.hpp"

#include "options.hpp"
#include "topic_client.hpp"

using namespace squawkbus::io;
namespace logging = squawkbus::logging;
using squawkbus::client::Options;
using squawkbus::client::TopicClient;
using squawkbus::messages::AuthenticationRequest;
using squawkbus::serialization::FrameBuffer;

namespace
{
  volatile std::sig_atomic_t last_signal = 0;
}

std::shared_ptr<SslContext> make_ssl_context(std::optional<std::string> capath)
{
  print_line("making ssl client context");
  auto ctx = std::make_shared<SslClientContext>();
  ctx->min_proto_version(TLS1_2_VERSION);
  if (capath.has_value())
  {
    print_line(std::format("Adding verify locations \"{}\"", capath.value()));
    ctx->load_verify_locations(capath.value());
  }
  else
  {
    print_line("setting default verify paths");
    ctx->set_default_verify_paths();
  }
  print_line("require ssl verification");
  ctx->verify();

  return ctx;
}


int main(int argc, char** argv)
{
  auto options = Options::parse(argc, argv);

  try
  {
    std::optional<std::shared_ptr<SslContext>> ssl_ctx;
    
    if (options.tls)
    {
      ssl_ctx = make_ssl_context(options.capath);
    }

    print_line(std::format(
      "connecting to host {} on port {}{}.",
      options.host,
      options.port,
      (options.tls ? " using tls" : "")));

    auto client_socket = std::make_shared<TcpClientSocket>();
    client_socket->connect(options.host, options.port);
    client_socket->blocking(false);

    AuthenticationRequest authentication_request;
    if (!options.authentication)
    {
      authentication_request.method = "NONE";
    }
    else
    {
      authentication_request.method = "HTPASSWD";
      FrameBuffer frame;
      frame << options.authentication->username << options.authentication->password;
      authentication_request.data = std::vector<char>(frame);
    }
    auto client = std::make_shared<TopicClient>(
      client_socket,
      std::move(authentication_request));
    auto poller = Poller(client);

    if (!ssl_ctx)
    {
      poller.add_handler(
        std::make_unique<TcpSocketPollHandler>(client_socket, 8096, 8096),
        options.host,
        options.port);
    }
    else
    {
      poller.add_handler(
        std::make_unique<TcpSocketPollHandler>(client_socket, *ssl_ctx, options.host, 8096, 8096),
        options.host,
        options.port);
    }

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