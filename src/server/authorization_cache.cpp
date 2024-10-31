#include "authorization_cache.hpp"

#include <cstdint>
#include <set>
#include <string>

#include "logging/log.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  bool AuthorizationCache::contains(const std::string& user, const std::string& topic, Role role) const
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

  const std::set<std::int32_t>& AuthorizationCache::get(
    const std::string& user,
    const std::string& topic,
    Role role)
  {
    return authorizations_[user][topic][role];
  }

  void AuthorizationCache::set(
    const std::string& user,
    const std::string& topic,
    Role role,
    const std::set<std::int32_t>& entitlements)
  {
    authorizations_[user][topic][role] = entitlements;
  }
}
