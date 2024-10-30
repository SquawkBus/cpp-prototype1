#include <algorithm>
#include <cstdint>
#include <format>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <stdexcept>
#include <vector>

#include "logging/log.hpp"

#include "authorization.hpp"
#include "authorization_yaml.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  AuthorizationSpec AuthorizationSpec::from(const std::string& line)
  {
    using namespace std::string_view_literals;

    auto args = std::views::split(line, ":"sv) | std::ranges::to<std::vector<std::string>>();
    if (args.size() != 4)
    {
      throw std::runtime_error(std::format("expected 4 parts, found {}.", args.size()));
    }

    auto topic_pattern = std::regex(args[0]);
    auto user_pattern = std::regex(args[1]);
    auto entitlements = args[2]
      | std::ranges::views::split(std::string_view(","sv))
      | std::ranges::views::transform([](auto &&i){
          return std::stoi(i | std::ranges::to<std::string>());
      })
      | std::ranges::to<std::set<std::int32_t>>();
    auto roles = std::ranges::fold_left(
      args[3]
      | std::ranges::views::split(std::string_view(","sv))
      | std::ranges::views::transform([](auto &&i){
          return std::ranges::to<std::string>(i);
      })
      | std::ranges::views::transform([](auto &&i){
          if (i == "Subscriber")
              return Role::Subscriber;
          else if (i == "Publisher")
              return Role::Publisher;
          else if (i == "Notifier")
              return Role::Notifier;
          else
              throw std::runtime_error("unknown role");
      }),
      Role::None,
      [](auto lhs, auto rhs) { return lhs | rhs; });

    return AuthorizationSpec {
      user_pattern,
      topic_pattern,
      entitlements,
      roles
    };
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
        if (!std::regex_match(user, spec.user_pattern()))
          continue;
        if (!std::regex_match(topic, spec.topic_pattern()))
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
    log.info(std::format("Loading authorizations from file \"{}\"", path.string()));

    YAML::Node yaml = YAML::LoadFile(path.string());
    auto config = yaml.as<std::map<std::string, std::map<std::string, squawkbus::server::Authorization>>>();

    std::vector<AuthorizationSpec> specs;
    for (auto& [user_pattern, authorizations] : config)
    {
      for (auto& [topic_pattern, authorization] : authorizations)
      {
        auto entitlements = std::set<int>(authorization.entitlements.begin(), authorization.entitlements.end());
        auto spec = AuthorizationSpec(
          std::regex(user_pattern),
          std::regex(topic_pattern),
          entitlements,
          authorization.role);

        specs.push_back(AuthorizationSpec());
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
      log.info("Using default authorizations");
      
      auto spec = AuthorizationSpec(
        std::regex(".*"),
        std::regex(".*"),
        std::set<std::int32_t> { 0 },
        Role::All
      );
      specs.push_back(spec);
    }

    return AuthorizationRepository(specs);
  }
}
