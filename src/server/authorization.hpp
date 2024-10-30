#ifndef SQUAWKBUS_SERVER_AUTHORIZATION_HPP
#define SQUAWKBUS_SERVER_AUTHORIZATION_HPP

#include <cstdint>
#include <filesystem>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

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

  class AuthorizationCache
  {
  private:
    std::map<std::string, std::map<std::string, std::map<Role, bool>>> authorizations_;

  public:
    std::optional<bool> get(const std::string& user, const std::string& topic, Role role) const
    {
      auto i_user = authorizations_.find(user);
      if (i_user == authorizations_.end())
        return std::nullopt;
      
      auto i_topic = i_user->second.find(topic);
      if (i_topic == i_user->second.end())
        return std::nullopt;

      auto i_role = i_topic->second.find(role);
      if (i_role == i_topic->second.end())
        return std::nullopt;

      return i_role->second;
    }

    void set(const std::string& user, const std::string& topic, Role role, bool is_authorized)
    {
      authorizations_[user][topic][role] = is_authorized;
    }
  };

  class AuthorizationManager
  {
  private:
    std::vector<AuthorizationSpec> specs_;
    mutable AuthorizationCache cache_;

  public:
    AuthorizationManager()
    {
    }
    AuthorizationManager(const std::vector<AuthorizationSpec>& specs)
      : specs_(specs)
    {
    }

    bool has_entitlement(const std::string& user, const std::string& topic, Role role) const;
    static AuthorizationManager load(const std::filesystem::path& path);
    static AuthorizationManager make(
      const std::optional<std::filesystem::path>& path,
      const std::vector<AuthorizationSpec>& specs);
  };
}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_HPP
