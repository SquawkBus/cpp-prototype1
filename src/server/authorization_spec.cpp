#include "authorization_spec.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "logging/log.hpp"
#include "constants.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger(LOGGER_NAME);
  }

  AuthorizationSpec AuthorizationSpec::from(const std::string& line)
  {
    using namespace std::string_view_literals;

    auto args = std::views::split(line, ":"sv) | std::ranges::to<std::vector<std::string>>();
    if (args.size() != 4)
    {
      throw std::runtime_error(std::format("expected 4 parts, found {}.", args.size()));
    }

    auto topic_pattern = args[0];
    auto user_pattern = args[1];
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
}
