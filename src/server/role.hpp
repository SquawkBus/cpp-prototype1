#ifndef SQUAWKBUS_SERVER_ROLE_HPP
#define SQUAWKBUS_SERVER_ROLE_HPP

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace squawkbus::server {
  
  enum class Role : int
  {
    None        = 0x00,
    
    Subscriber  = 0x01,
    Publisher   = 0x02,
    Notifier    = 0x04,

    All         = 0x07
  };

  inline constexpr Role
  operator&(Role x, Role y)
  {
    return static_cast<Role>(static_cast<int>(x) & static_cast<int>(y));
  }

  inline constexpr Role
  operator|(Role x, Role y)
  {
    return static_cast<Role>(static_cast<int>(x) | static_cast<int>(y));
  }

  inline constexpr Role
  operator^(Role x, Role y)
  {
    return static_cast<Role>(static_cast<int>(x) ^ static_cast<int>(y));
  }

  inline constexpr Role
  operator~(Role x)
  {
    return static_cast<Role>(~static_cast<int>(x));
  }

  inline Role &
  operator&=(Role & x, Role y)
  {
    x = x & y;
    return x;
  }

  inline Role &
  operator|=(Role & x, Role y)
  {
    x = x | y;
    return x;
  }

  inline Role &
  operator^=(Role & x, Role y)
  {
    x = x ^ y;
    return x;
  }

  inline std::string
  to_string(Role role)
  {
    using namespace std::string_view_literals;

    auto roles = std::vector<std::pair<Role, std::string_view>> {
      { Role::Subscriber, "Subscriber"sv },
      { Role::Publisher, "Publisher"sv },
      { Role::Notifier, "Notifier"sv },
    };
    
    auto s = std::ranges::fold_left(
      roles
        | std::views::filter([role](auto&& x) {return (x.first & role) == x.first; })
        | std::views::transform([](auto&& x) { return x.second; })
        | std::views::join_with(" | "sv),
      std::string{}, std::plus<>{});

    return s;
  }
}

#endif // SQUAWKBUS_SERVER_ROLE_HPP
