#ifndef SQUAWKBUS_SERVER_ROLE_HPP
#define SQUAWKBUS_SERVER_ROLE_HPP

#include <string>
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
}

#endif // SQUAWKBUS_SERVER_ROLE_HPP
