#ifndef SQUAWKBUS_SERVER_AUTHENTICATION_REPOSITORY_HPP
#define SQUAWKBUS_SERVER_AUTHENTICATION_REPOSITORY_HPP

#include <map>
#include <string>

namespace squawkbus::server
{
  class AuthenticationRepository
  {
  private:
    std::map<std::string, std::string> entries_;
  public:
    AuthenticationRepository()
    {
    }
    AuthenticationRepository(std::map<std::string, std::string>&& entries)
      : entries_(entries)
    {
    }

    bool authenticate(const std::string& username, const std::string& password) const;
  };
}

#endif // SQUAWKBUS_SERVER_AUTHENTICATION_REPOSITORY_HPP
