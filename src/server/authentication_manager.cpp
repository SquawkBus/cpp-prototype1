#include "authentication_manager.hpp"
#include "authentication_repository.hpp"

#include <format>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

#include "logging/log.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  void AuthenticationManager::load()
  {
    if (!password_file_)
      return;

    log.info(std::format("Loading password file {}", *password_file_));

    auto file = std::fstream(*password_file_, std::ios::in);
    if (!file.is_open())
      throw std::runtime_error("failed to open password file");

    std::map<std::string, std::string> entries;
    std::string line;
    while (std::getline(file, line))
    {
      // Skip comments.
      if (line.starts_with("#"))
        continue;

      // Split user and data.
      auto colon = line.find(':');
      if (colon == std::string::npos)
        throw std::runtime_error("invalid password record");

      auto user = line.substr(0, colon);
      auto data = line.substr(colon+1);

      entries.insert({user, data});
    }

    repository_ = AuthenticationRepository(std::move(entries));
  }

  bool AuthenticationManager::authenticate(const std::string& username, const std::string& password) const
  {
    log.debug(std::format("Authenticating \"{}\"", username));
    
    return repository_.authenticate(username, password);
  }
}
