#ifndef SQUAWKBUS_SERVER_CMD_LINE_HPP
#define SQUAWKBUS_SERVER_CMD_LINE_HPP

#include <filesystem>
#include <format>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include "authorization_spec.hpp"
#include "io/endpoint.hpp"

namespace squawkbus::server
{
  using squawkbus::io::Endpoint;
  
  struct TLSOptions
  {
    std::filesystem::path certfile;
    std::filesystem::path keyfile;
  };

  struct Options
  {
    Endpoint endpoint;
    std::optional<TLSOptions> tls;
    std::vector<AuthorizationSpec> authorizations;
    std::optional<std::filesystem::path> authorizations_file;
    std::optional<std::filesystem::path> password_file;

    static std::string usage(std::string program_name);
    static Options parse(int argc, const char** argv);
  };
}

#endif // SQUAWKBUS_SERVER_CMD_LINE_HPP
