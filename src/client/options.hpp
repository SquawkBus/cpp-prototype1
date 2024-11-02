#ifndef SQUAWKBUS_CLIENT_OPTIONS_HPP
#define SQUAWKBUS_CLIENT_OPTIONS_HPP

#include <cstdint>
#include <optional>
#include <string>

namespace squawkbus::client
{
  enum class AuthenticationMethod : int
  {
    None = 0,
    Htpasswd
  };

  struct Options
  {
    std::string host;
    std::uint16_t port;
    bool tls;
    std::optional<std::string> capath;
    AuthenticationMethod authentication_method;
    std::optional<std::string> username;
    std::optional<std::string> password;

    static std::string usage(const std::string& progname);
    static Options parse(int argc, char** argv);
  };
}

#endif // SQUAWKBUS_CLIENT_OPTIONS_HPP
