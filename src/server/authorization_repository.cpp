#include "authorization_repository.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>

#include "logging/log.hpp"
#include "utils/utils.hpp"

#include "authorization_yaml.hpp"
#include "constants.hpp"
#include "role.hpp"

namespace squawkbus::server
{
  using namespace std::string_view_literals;

  namespace
  {
    auto log = logging::logger(LOGGER_NAME);

    inline std::string format_spec(const AuthorizationSpec& spec)
    {
      return std::format(
        "Authorizing user \"{}\" on topic \"{}\" for role {} with entitlements [{}]",
        spec.user_pattern(),
        spec.topic_pattern(),
        to_string(spec.roles()),
        std::ranges::fold_left(
          std::vector<int> {spec.entitlements().begin(), spec.entitlements().end()}
            | std::views::transform([](auto&& x) { return std::to_string(x); })
            | std::views::join_with(", "sv),
          std::string{}, std::plus<>{}));
    }
  }

  const std::set<std::int32_t>& AuthorizationRepository::entitlements(
    const std::string& user,
    const std::string& topic,
    Role role) const
  {
    if (!cache_.contains(user, topic, role))
    {
      for (auto& spec : specs_)
      {
        if (!std::regex_match(user, spec.user_regex()))
          continue;
        if (!std::regex_match(topic, spec.topic_regex()))
          continue;
        if ((role & spec.roles()) != role)
          continue;

        cache_.set(user, topic, role, spec.entitlements());
        break;
      }
    }

    return cache_.get(user, topic, role);
  }

  AuthorizationRepository AuthorizationRepository::load(const std::filesystem::path& path)
  {
    log.info(std::format("Loading authorizations from file \"{}\".", path.string()));

    YAML::Node yaml = YAML::LoadFile(path.string());
    auto config = yaml.as<std::map<std::string, std::map<std::string, squawkbus::server::Authorization>>>();

    std::vector<AuthorizationSpec> specs;
    for (auto& [user_pattern, authorizations] : config)
    {
      for (auto& [topic_pattern, authorization] : authorizations)
      {
        auto entitlements = std::set<int>(
          authorization.entitlements.begin(),
          authorization.entitlements.end());
        auto spec = AuthorizationSpec(
          user_pattern,
          topic_pattern,
          entitlements,
          authorization.role);

        log.debug(format_spec(spec));

        specs.push_back(spec);
      }
    }

    return AuthorizationRepository(specs);
  }

  AuthorizationRepository AuthorizationRepository::make(
    const std::optional<std::filesystem::path>& path,
    const std::vector<AuthorizationSpec>& cmd_line_specs)
  {
    if (path)
    {
      return AuthorizationRepository::load(*path);
    }

    auto specs = cmd_line_specs; // copy the command line specs;
    if (specs.empty())
    {
      log.info("Using default authorizations.");
      
      auto spec = AuthorizationSpec(
        ".*",
        ".*",
        std::set<std::int32_t> { 0 },
        Role::All);

      log.debug(format_spec(spec));

      specs.push_back(spec);
    }

    return AuthorizationRepository(specs);
  }
}
