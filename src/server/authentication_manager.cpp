#include "authentication_manager.hpp"
#include "authentication_repository.hpp"

#include <format>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

#include "logging/log.hpp"

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "messages/messages.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  using squawkbus::messages::Authenticate;
  using squawkbus::serialization::FrameBuffer;

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

  std::optional<std::string> AuthenticationManager::authenticate(Authenticate&& message) const
  {
    log.debug(std::format("Authenticating \"{}\"", message.method()));

    if (message.method() == "NONE")
    {
      return "nobody";
    }

    if (message.method() == "HTPASSWD")
    {
      auto frame = FrameBuffer::from(std::move(message.data()));
      std::string username, password;
      frame >> username >> password;
      if (!repository_.authenticate(username, password))
        return std::nullopt;

      return username;
    }

    return std::nullopt;
  }
}
