#ifndef SQUAWKBUS_SERVER_AUTHENTICATION_MANAGER_HPP
#define SQUAWKBUS_SERVER_AUTHENTICATION_MANAGER_HPP

#include <optional>
#include <string>
#include <utility>

#include "messages/messages.hpp"

#include "authentication_repository.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::Authenticate;

  class AuthenticationManager
  {
  private:
    AuthenticationRepository repository_;
    std::optional<std::string> password_file_;

  public:
    AuthenticationManager(const std::optional<std::string>& password_file)
      : password_file_(password_file)
    {
    }
    AuthenticationManager(AuthenticationManager&& other)
    {
      repository_ = std::move(other.repository_);
      password_file_ = std::move(other.password_file_);
    }

    void load();
    std::optional<std::string> authenticate(Authenticate&& message) const;
  };
}

#endif // SQUAWKBUS_SERVER_AUTHENTICATION_MANAGER_HPP
