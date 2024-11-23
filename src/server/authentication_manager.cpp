#include "authentication_manager.hpp"
#include "authentication_repository.hpp"

#include <format>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

#include "logging/log.hpp"

#include "serialization/frame_reader.hpp"
#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"
#include "serialization/basic_token.hpp"

#include "messages/messages.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  using squawkbus::messages::AuthenticationRequest;
  using squawkbus::serialization::FrameBuffer;
  using squawkbus::serialization::FrameReader;
  using squawkbus::serialization::decode_basic_token;

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

  std::optional<std::string> AuthenticationManager::authenticate(AuthenticationRequest&& message) const
  {
    log.debug(std::format("Authenticating \"{}\"", message.method));

    if (message.method == "none")
    {
      return authenticate_none(message);
    }
    else if (message.method == "basic")
    {
      return authenticate_basic(message);
    }
    else
    {
      return std::nullopt;
    }
  }

  std::optional<std::string> AuthenticationManager::authenticate_none(AuthenticationRequest& message) const
  {
      return "nobody";
  }

  std::optional<std::string> AuthenticationManager::authenticate_basic(AuthenticationRequest& message) const
  {
    auto [username, password] = decode_basic_token(message.data);
    if (!repository_.authenticate(username, password))
      return std::nullopt;

    return username;
  }

}
