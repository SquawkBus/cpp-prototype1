#ifndef SQUAWKBUS_CLIENT_OPTIONS_HPP
#define SQUAWKBUS_CLIENT_OPTIONS_HPP

#include <cstdint>
#include <string>

namespace squawkbus::client
{
  struct Options
  {
    std::string host;
    std::uint16_t port;
    bool tls;
    std::string capath;

    static std::string usage(const std::string& progname);
    static Options parse(int argc, char** argv);
  };
}

#endif // SQUAWKBUS_CLIENT_OPTIONS_HPP
