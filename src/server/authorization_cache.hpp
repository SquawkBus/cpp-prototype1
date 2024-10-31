#ifndef SQUAWKBUS_SERVER_AUTHORIZATION_CACHE_HPP
#define SQUAWKBUS_SERVER_AUTHORIZATION_CACHE_HPP

#include <cstdint>
#include <map>
#include <set>
#include <string>

#include "role.hpp"

namespace squawkbus::server
{
  class AuthorizationCache
  {
  private:
    std::map<std::string, std::map<std::string, std::map<Role, std::set<std::int32_t>>>> authorizations_;

  public:
    bool contains(const std::string& user, const std::string& topic, Role role) const;

    const std::set<std::int32_t>& get(
      const std::string& user,
      const std::string& topic,
      Role role);

    void set(
      const std::string& user,
      const std::string& topic,
      Role role,
      const std::set<std::int32_t>& entitlements);
  };
}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_CACHE_HPP
