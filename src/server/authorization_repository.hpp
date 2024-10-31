#ifndef SQUAWKBUS_SERVER_AUTHORIZATION_REPOSITORY_HPP
#define SQUAWKBUS_SERVER_AUTHORIZATION_REPOSITORY_HPP

#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "role.hpp"
#include "authorization_spec.hpp"
#include "authorization_cache.hpp"

namespace squawkbus::server
{
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
}

#endif // SQUAWKBUS_SERVER_AUTHORIZATION_REPOSITORY_HPP
