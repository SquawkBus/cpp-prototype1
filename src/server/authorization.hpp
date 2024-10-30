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

    void reload()
    {
      repository_ = AuthorizationRepository::make(path_, cmd_line_specs_);
    }

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
