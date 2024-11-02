#include "options.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>

namespace squawkbus::client
{
  std::string Options::usage(const std::string& progname)
  {
    std::stringstream ss;

    ss
      << "Usage: " << progname << "\n"
      << "\t" << "--host <host>" << "\n"
      << "\t" << "--port <port>" << "\n"
      << "\t" << "--tls" << "\n"
      << "\t" << "[--capath <file>]" << "\n"
      << "\t" << "[--authentication-method <none | htpasswd>]" << "\n"
      << "\t" << "[--username <username>]" << "\n"
      << "\t" << "[--password <password>]" << "\n"
      ;
    return ss.str();
  }

  Options Options::parse(int argc, char** argv)
  {
    try
    {
      Options options;

      int argi = 1;
      while (argi < argc)
      {
        auto arg = std::string(argv[argi++]);

        if (arg == "--port")
        {
          if (options.port != 0)
            throw std::runtime_error(std::format("duplicate arg {}", arg));
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires 1 arg", arg));
          auto port = std::stoi(argv[argi++]);
          if (port <= 0 || port > std::numeric_limits<std::uint16_t>::max())
            throw std::runtime_error(std::format("invalid argument for {}", arg));
          options.port = static_cast<std::uint16_t>(port);
        }
        else if (arg == "--host")
        {
          if (!options.host.empty())
            throw std::runtime_error(std::format("duplicate arg {}", arg));
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires 1 arg", arg));
          options.host = argv[argi++];
        }
        else if (arg == "--tls")
        {
          options.tls = true;
        }
        else if (arg == "--capath")
        {
          if (options.capath != std::nullopt)
            throw std::runtime_error(std::format("duplicate arg {}", arg));
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires 1 arg", arg));
          options.capath = argv[argi++];
        }
        else if (arg == "--authentication-method")
        {
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires 1 arg", arg));
          arg = std::string(argv[argi++]);
          if (arg == "none")
            options.authentication_method = AuthenticationMethod::None;
          else if (arg == "htpasswd")
            options.authentication_method = AuthenticationMethod::Htpasswd;
          else
            throw std::runtime_error(std::format("invalid argument {}", arg));
        }
        else if (arg == "--username")
        {
          if (options.username != std::nullopt)
            throw std::runtime_error(std::format("duplicate arg {}", arg));
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires 1 arg", arg));
          options.username = argv[argi++];
        }
        else if (arg == "--password")
        {
          if (options.password != std::nullopt)
            throw std::runtime_error(std::format("duplicate arg {}", arg));
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires 1 arg", arg));
          options.password = argv[argi++];
        }
        else
        {
          throw std::runtime_error(std::format("invalid argument {}", arg));
        }
      }

      return options;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      std::cerr << usage(argv[0]);
      exit(1);
    }    
  }
}
