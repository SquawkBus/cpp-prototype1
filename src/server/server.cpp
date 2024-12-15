#include <signal.h>

#include <csignal>
#include <cstdio>
#include <format>
#include <set>
#include <utility>

#include "io/poller.hpp"
#include "io/tcp_listener_poll_handler.hpp"
#include "io/ssl_ctx.hpp"
#include "logging/log.hpp"
#include "utils/utils.hpp"

#include "options.hpp"
#include "distributor.hpp"
#include "authentication_manager.hpp"
#include "authorization_manager.hpp"

using namespace squawkbus::io;
namespace logging = squawkbus::logging;

using squawkbus::server::AuthenticationManager;
using squawkbus::server::AuthorizationManager;
using squawkbus::server::Distributor;
using squawkbus::io::Endpoint;
using squawkbus::server::Options;

namespace
{
  volatile std::sig_atomic_t last_signal = 0;
}

void signal_handler(int signal)
{
  logging::info(std::format("Received signal {}", signal));
  
  last_signal = signal;
}

std::shared_ptr<SslContext> make_ssl_context(const std::string& certfile, const std::string& keyfile)
{
  logging::debug("Configuring TLS.");
  auto ctx = std::make_shared<SslServerContext>();
  ctx->min_proto_version(TLS1_2_VERSION);
  logging::debug(std::format("Adding certificate file \"{}\"", certfile));
  ctx->use_certificate_file(certfile);
  logging::debug(std::format("Adding key file \"{}\"", keyfile));
  ctx->use_private_key_file(keyfile);
  return ctx;
}

void start_server(
  const Endpoint& endpoint,
  AuthenticationManager&& authentication_manager,
  AuthorizationManager&& authorization_manager,
  std::optional<std::shared_ptr<SslContext>> ssl_ctx)
{
  auto poll_client = std::make_shared<Distributor>(
    std::move(authentication_manager),
    std::move(authorization_manager));
  auto poller = Poller(poll_client);
  poller.add_handler(
    std::make_unique<TcpListenerPollHandler>(endpoint.port(), ssl_ctx),
    endpoint.host(),
    endpoint.port());
  poller.event_loop(last_signal);
}

int main(int argc, const char** argv)
{
  std::signal(SIGHUP, signal_handler);

  try
  {
    auto options = Options::parse(argc, argv);

    logging::info(
      std::format(
        "Starting squawkbus on endpoint {}{}.",
        std::string(options.endpoint),
        (options.tls == std::nullopt ? "" : " with TLS")));

    std::optional<std::shared_ptr<SslContext>> ssl_ctx;

    if (options.tls != std::nullopt)
    {
      ssl_ctx = make_ssl_context(options.tls->certfile, options.tls->keyfile);
    }

    auto authentication_manager = AuthenticationManager(options.password_file);

    auto authorization_manager = AuthorizationManager(
      options.authorizations_file,
      options.authorizations);

    start_server(
      options.endpoint,
      std::move(authentication_manager),
      std::move(authorization_manager),
      std::move(ssl_ctx));
  }
  catch(const std::exception& error)
  {
    logging::error(std::format("Server failed: {}", error.what()));
  }
  catch (...)
  {
    logging::error("Unknown error.");
  }

  logging::info("Stopped.");


  return 0;
}
