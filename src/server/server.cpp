#include <signal.h>

#include <cstdio>
#include <format>
#include <set>

#include "io/poller.hpp"
#include "io/tcp_listener_poll_handler.hpp"
#include "io/ssl_ctx.hpp"
#include "logging/log.hpp"
#include "utils/utils.hpp"

#include "cmd_line.hpp"
#include "distributor.hpp"

using namespace squawkbus::io;
namespace logging = squawkbus::logging;

using squawkbus::server::AuthorizationManager;
using squawkbus::server::Distributor;
using squawkbus::io::Endpoint;
using squawkbus::server::Options;

std::shared_ptr<SslContext> make_ssl_context(const std::string& certfile, const std::string& keyfile)
{
  logging::info("making ssl server context");
  auto ctx = std::make_shared<SslServerContext>();
  ctx->min_proto_version(TLS1_2_VERSION);
  logging::info(std::format("Adding certificate file \"{}\"", certfile));
  ctx->use_certificate_file(certfile);
  logging::info(std::format("Adding key file \"{}\"", keyfile));
  ctx->use_private_key_file(keyfile);
  return ctx;
}

void start_server(const Options& options, std::optional<std::shared_ptr<SslContext>> ssl_ctx)
{
  auto poll_client = std::make_shared<Distributor>(options.authorizations_file, options.authorizations);
  auto poller = Poller(poll_client);
  poller.add_handler(
    std::make_unique<TcpListenerPollHandler>(options.endpoint.port(), ssl_ctx),
    options.endpoint.host(),
    options.endpoint.port());
  poller.event_loop();
}

int main(int argc, const char** argv)
{
  // signal(SIGPIPE,SIG_IGN);

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

    auto authorization_manager = AuthorizationManager::make(
      options.authorizations_file,
      options.authorizations);

    start_server(options, std::move(ssl_ctx));
  }
  catch(const std::exception& error)
  {
    logging::error(std::format("Server failed: {}", error.what()));
  }
  catch (...)
  {
    logging::error(std::format("unknown error"));
  }

  logging::info("server stopped");


  return 0;
}
