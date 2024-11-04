#ifndef SQUAWKBUS_CLIENT_OPTIONS_HPP
#define SQUAWKBUS_CLIENT_OPTIONS_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "io/ssl_ctx.hpp"
#include "messages/messages.hpp"

namespace squawkbus::client
{
  using squawkbus::io::SslContext;
  using squawkbus::messages::AuthenticationRequest;

  struct AuthenticationOption
  {
    std::string username;
    std::string password;
  };

  struct Options
  {
    std::string host;
    std::uint16_t port = 0;
    bool tls = false;
    std::optional<std::string> capath;
    std::optional<AuthenticationOption> authentication;

    static std::string usage(const std::string& progname);
    static Options parse(int argc, char** argv);

    std::optional<std::shared_ptr<SslContext>> make_ssl_context() const;
    AuthenticationRequest make_authentication_request() const;
  };
}

#endif // SQUAWKBUS_CLIENT_OPTIONS_HPP
