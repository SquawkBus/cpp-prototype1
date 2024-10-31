#ifndef SQUAWKBUS_SERVER_AUTHORIZATION_SPEC_HPP
#define SQUAWKBUS_SERVER_AUTHORIZATION_SPEC_HPP

#include <cstdint>
#include <regex>
#include <set>
#include <string>

#include "role.hpp"

namespace squawkbus::server
{
  class AuthorizationSpec
  {
  private:
    std::regex user_pattern_;
    std::regex topic_pattern_;
    std::set<std::int32_t> entitlements_;
    Role roles_;

  public:
    AuthorizationSpec()
    {
    }
    AuthorizationSpec(
      const std::regex& user_pattern,
      const std::regex& topic_pattern,
      const std::set<std::int32_t>& entitlements,
      Role roles)
      : user_pattern_(user_pattern),
        topic_pattern_(topic_pattern),
        entitlements_(entitlements),
        roles_(roles)
    {
    }

    const std::regex user_pattern() const noexcept { return user_pattern_; }
    const std::regex topic_pattern() const noexcept { return topic_pattern_; }
    const std::set<std::int32_t> entitlements() const noexcept { return entitlements_; }
    Role roles() const noexcept { return roles_; };

    static AuthorizationSpec from(const std::string& line);
  };
}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_SPEC_HPP
