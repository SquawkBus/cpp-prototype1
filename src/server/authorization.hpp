#ifndef SQUAWKBUS_SERVER_AUTHORIZATION_HPP
#define SQUAWKBUS_SERVER_AUTHORIZATION_HPP

#include <cstdint>
#include <filesystem>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "role.hpp"
#include "authorization_spec.hpp"

namespace squawkbus::server
{
  class AuthorizationCache
  {
  private:
    std::map<std::string, std::map<std::string, std::map<Role, std::set<std::int32_t>>>> authorizations_;

  public:
    bool contains(const std::string& user, const std::string& topic, Role role) const
    {
      auto i_user = authorizations_.find(user);
      if (i_user == authorizations_.end())
        return false;
      
      auto i_topic = i_user->second.find(topic);
      if (i_topic == i_user->second.end())
        return false;

      auto i_role = i_topic->second.find(role);
      if (i_role == i_topic->second.end())
        return false;

      return true;
    }

    const std::set<std::int32_t>& get(
      const std::string& user,
      const std::string& topic,
      Role role)
    {
      return authorizations_[user][topic][role];
    }

    void set(
      const std::string& user,
      const std::string& topic,
      Role role,
      const std::set<std::int32_t>& entitlements)
    {
      authorizations_[user][topic][role] = entitlements;
    }
  };

  class AuthorizationRepository
  {
  private:
    std::vector<AuthorizationSpec> specs_;
    mutable AuthorizationCache cache_;

  public:
    AuthorizationRepository()
    {
    }
    AuthorizationRepository(const std::vector<AuthorizationSpec>& specs)
      : specs_(specs)
    {
    }

    const std::set<std::int32_t>& entitlements(
      const std::string& user,
      const std::string& topic,
      Role role) const;
    static AuthorizationRepository load(const std::filesystem::path& path);
    static AuthorizationRepository make(
      const std::optional<std::filesystem::path>& path,
      const std::vector<AuthorizationSpec>& specs);
  };

  class AuthorizationManager
  {
  private:
    std::optional<std::filesystem::path> path_;
    std::vector<AuthorizationSpec> cmd_line_specs_;
    AuthorizationRepository repository_;

  public:
    AuthorizationManager(
      const std::optional<std::filesystem::path>& path,
      const std::vector<AuthorizationSpec>& cmd_line_specs)
      : path_(path),
        cmd_line_specs_(cmd_line_specs)
    {
    }

    void reload();

    const std::set<std::int32_t>& entitlements(
      const std::string& user,
      const std::string& topic,
      Role role) const
    {
      return repository_.entitlements(user, topic, role);
    }

  };
}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_HPP
