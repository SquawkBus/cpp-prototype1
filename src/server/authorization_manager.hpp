#ifndef SQUAWKBUS_SERVER_AUTHORIZATION_MANAGER_HPP
#define SQUAWKBUS_SERVER_AUTHORIZATION_MANAGER_HPP

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
#include "authorization_repository.hpp"

namespace squawkbus::server
{
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

    void load();

    const std::set<std::int32_t>& entitlements(
      const std::string& user,
      const std::string& topic,
      Role role) const
    {
      return repository_.entitlements(user, topic, role);
    }

  };
}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_MANAGER_HPP
