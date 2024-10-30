#ifndef SQUAWKBUS_SERVER_AUTHORIZATION_HPP
#define SQUAWKBUS_SERVER_AUTHORIZATION_HPP

#include <cstdint>
#include <regex>
#include <set>
#include <string>

#include "role.hpp"

namespace squawkbus::server
{
  struct Authorization
  {
    std::vector<int> entitlements;
    Role role;
  };

  struct AuthorizationSpec
  {
    std::regex user_pattern;
    std::regex topic_pattern;
    std::set<std::int32_t> entitlements;
    Role roles;

    static AuthorizationSpec from(const std::string& line);
  };

}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_HPP
