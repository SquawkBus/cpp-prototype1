#include "cmd_line.hpp"

#include <filesystem>
#include <format>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

namespace squawkbus::server
{
  using squawkbus::io::Endpoint;
  
  std::string Options::usage(std::string program_name)
  {
    std::stringstream ss;
    ss
      << "Usage " << program_name << "[OPTION...]" << "\n"
      << "\t" << "--endpoint <host:port>" << "\n"
      << "\t" << "--tls <certfile> <keyfile>" << "\n"
      << "\t" << "--authorizations-file <file>" << "\n"
      << "\t" << "--password-file <file>" << "\n"
      << "\t" << "--authorization <user:topic:entitlements:roles>" << "\n"
      ;
    return ss.str();
  }

  Options Options::parse(int argc, const char** argv)
  {
    try
    {
      auto options = Options {};

      auto argi = 1;
      while (argi < argc)
      {
        auto arg = std::string(argv[argi++]);

        if (arg == "--endpoint")
        {
          if (!options.endpoint.empty())
            throw std::runtime_error(std::format("duplicate argument {}", arg));
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires an argument", arg));
          options.endpoint = Endpoint::parse(argv[argi++]);
        }
        else if (arg == "--tls")
        {
          if (options.tls != std::nullopt)
            throw std::runtime_error(std::format("duplicate argument {}", arg));
          if (argc - argi < 2)
            throw std::runtime_error(std::format("{} requires two arguments", arg));
          auto tls = TLSOptions{};
          tls.certfile = argv[argi++];
          tls.keyfile = argv[argi++];
          options.tls = tls;
        }
        else if (arg == "--authorizations-file")
        {
          if (options.authorizations_file != std::nullopt)
            throw std::runtime_error(std::format("duplicate argument {}", arg));
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires an argument", arg));
          options.authorizations_file = argv[argi++];
        }
        else if (arg == "--password-file")
        {
          if (options.password_file != std::nullopt)
            throw std::runtime_error(std::format("duplicate argument {}", arg));
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires an argument", arg));
          options.password_file = argv[argi++];
        }
        else if (arg == "--authorization")
        {
          if (argc - argi < 1)
            throw std::runtime_error(std::format("{} requires an argument", arg));
          auto authorization_spec = AuthorizationSpec::from(arg);
          options.authorizations.push_back(authorization_spec);
        }
      }

      if (options.endpoint.empty())
        options.endpoint = Endpoint("0.0.0.0", 8558);

      return options;
    }
    catch(const std::exception& e)
    {
      std:: cerr << e.what() << "\n";
      std::cerr << usage(argv[0]);
      throw;
    }
  }
}
