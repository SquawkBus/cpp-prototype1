#ifndef SQUAWKBUS_CLIENT_OPTIONS_HPP
#define SQUAWKBUS_CLIENT_OPTIONS_HPP

#include <cstdint>
#include <optional>
#include <string>

namespace squawkbus::client
{
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
  };
}

#endif // SQUAWKBUS_CLIENT_OPTIONS_HPP
