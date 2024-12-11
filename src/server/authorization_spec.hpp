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
    std::string user_pattern_;
    std::string topic_pattern_;
    std::regex user_regex_;
    std::regex topic_regex_;
    std::set<std::int32_t> entitlements_;
    Role roles_;

  public:
    AuthorizationSpec()
    {
    }
    AuthorizationSpec(
      const std::string& user_pattern,
      const std::string& topic_pattern,
      const std::set<std::int32_t>& entitlements,
      Role roles)
      : user_pattern_(user_pattern),
        topic_pattern_(topic_pattern),
        user_regex_(user_pattern),
        topic_regex_(topic_pattern),
        entitlements_(entitlements),
        roles_(roles)
    {
    }

    const std::string& user_pattern() const noexcept { return user_pattern_; }
    const std::string& topic_pattern() const noexcept { return topic_pattern_; }
    const std::regex& user_regex() const noexcept { return user_regex_; }
    const std::regex& topic_regex() const noexcept { return topic_regex_; }
    const std::set<std::int32_t>& entitlements() const noexcept { return entitlements_; }
    Role roles() const noexcept { return roles_; };

    static AuthorizationSpec from(const std::string& line);
  };
}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_SPEC_HPP
