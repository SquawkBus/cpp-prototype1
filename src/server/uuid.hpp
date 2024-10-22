#ifndef SQUAWKBUS_SERVER_UUID_HPP
#define SQUAWKBUS_SERVER_UUID_HPP

extern "C"
{
#include <uuid/uuid.h>
}

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

namespace squawkbus::server
{
  class uuid
  {
  private:
    uuid_t data_;

  public:
    uuid()
    {
      uuid_clear(data_);
    }

    uuid(uuid_t data)
    {
      uuid_copy(data_, data);
    }
    uuid(const uuid& other)
    {
      uuid_copy(data_, other.data_);
    }
    uuid operator=(const uuid& other)
    {
      uuid_copy(this->data_, other.data_);
      return *this;
    }

    uuid_t& data() noexcept { return data_; }
    const uuid_t& data() const noexcept { return data_; }

    std::string str() const noexcept
    {
      char s[37];
      uuid_unparse(data_, s);
      return std::string(s, s + 36);
    }

    explicit operator std::string() const
    {
      return str();
    }

    bool operator==(const uuid& other) const noexcept
    {
      return uuid_compare(data_, other.data_) == 0;
    }

    bool operator!=(const uuid& other) const noexcept
    {
      return uuid_compare(data_, other.data_) != 0;
    }

    bool operator<=(const uuid& other) const noexcept
    {
      return uuid_compare(data_, other.data_) <= 0;
    }

    bool operator<(const uuid& other) const noexcept
    {
      return uuid_compare(data_, other.data_) < 0;
    }

    bool operator>(const uuid& other) const noexcept
    {
      return uuid_compare(data_, other.data_) > 0;
    }

    bool operator>=(const uuid& other) const noexcept
    {
      return uuid_compare(data_, other.data_) >= 0;
    }

    static uuid generate()
    {
      uuid_t data;
      uuid_clear(data);
      uuid_generate(data);
      return uuid(data);
    }

    static uuid parse(const std::string& s)
    {
      uuid u;
      if (uuid_parse(s.c_str(), u.data()) != 0)
        throw std::runtime_error("failed to parse uuid");

      return u;
    }

  };

  std::ostream& operator<<(std::ostream& os, const uuid& u)
  {
    return os << u.str();
  }
}

#endif // SQUAWKBUS_SERVER_UUID_HPP
